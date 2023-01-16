package main

import (
	"log"
	"net/http"
)

func main() {
	res, err := http.Head("http://localhost:18888")

	if err != nil {
		panic(err)
	}

	log.Println("Status: ", res.Status)
	log.Println("Headers: ", res.Header)
}
