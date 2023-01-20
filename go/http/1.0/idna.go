package main

import (
	"fmt"
	"golang.org/x/net/idna"
)

func main() {
	domain := "国際化ドメイン"

	ascii, err := idna.ToASCII(domain)

	if err != nil {
		panic(err)
	}

	fmt.Printf("%s -> %s", domain, ascii)
}
