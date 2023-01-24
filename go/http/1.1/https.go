package main

import (
	"fmt"
	"log"
	"net/http"
	"net/http/httputil"
)

func main() {
	http.HandleFunc("/", handler)
	log.Println("Listen 18443 ...")

	err := http.ListenAndServeTLS(":18443", "server.crt", "server.key", nil)

	log.Println(err)
}

func handler(w http.ResponseWriter, r *http.Request) {
	dump, err := httputil.DumpRequest(r, true)

	if err != nil {
		http.Error(w, fmt.Sprint(err), http.StatusInternalServerError)
		return
	}

	fmt.Println(string(dump))
	fmt.Fprintf(w, "<html><body>HTTP/1.1</body></html>\n")
}
