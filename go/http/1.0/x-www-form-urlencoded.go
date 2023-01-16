package main

import (
	"log"
	"net/http"
	"net/url"
)

func main() {
	values := url.Values{"key": {"value"}}

	res, err := http.PostForm("http://localhost:18888", values)

	if err != nil {
		panic(err)
	}

	defer res.Body.Close()

	log.Println("Status: ", res.Status)
	log.Println("Headers: ", res.Header)
}
