package main

import (
	"io/ioutil"
	"log"
	"net/http"
	"net/url"
)

func main() {
	queries := url.Values{"query": {"query parameter"}}

	res, err := http.Get("http://localhost:18888" + "?" + queries.Encode())

	if err != nil {
		panic(err)
	}

	defer res.Body.Close()

	body, err := ioutil.ReadAll(res.Body)

	if err != nil {
		panic(err)
	}

	log.Println("Status: ", res.Status)
	log.Println("Headers: ", res.Header)
	log.Println(string(body))
}
