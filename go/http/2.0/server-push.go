package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

var image []byte

func main() {
	http.HandleFunc("/", handlerHTML)
	http.HandleFunc("/image", handlerImage)

	fmt.Println("Listen (:18443) ...")

	err := http.ListenAndServeTLS(":18443", "server.crt", "server.key", nil)

	fmt.Println(err)
}

func init() {
	var err error

	image, err = ioutil.ReadFile("./image.png")

	if err != nil {
		panic(err)
	}
}

func handlerHTML(w http.ResponseWriter, r *http.Request) {
	pusher, ok := w.(http.Pusher)

	if ok {
		pusher.Push("/image", nil)
	}

	w.Header().Add("Content-Type", "text/html")

	fmt.Fprintf(w, `<html><body><img src="/image" alt="HTTP/2 server" width="128" height="128" /></body></html>`)
}

func handlerImage(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "image/png")
	w.Write(image)
}
