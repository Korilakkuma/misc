package main

import (
	"fmt"
	"net/http"
	"os"
)

func main() {
	res, err := http.Get(os.Args[1])

	if err != nil {
		panic(err)
	}

	defer res.Body.Close()

	fmt.Printf("Protocol Version: %s\n", res.Proto)
}
