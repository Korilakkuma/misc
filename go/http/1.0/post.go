package main

import (
	"log"
	"net/http"
	"os"
)

func main() {
	body, err := os.Open("./text.txt")

	if err != nil {
		panic(err)
	}

	res, err := http.Post("http://localhost:18888", "text/plain", body)

	if err != nil {
		panic(err)
	}

	defer res.Body.Close()

	log.Println("Status: ", res.Status)
	log.Println("Headers: ", res.Header)
}
