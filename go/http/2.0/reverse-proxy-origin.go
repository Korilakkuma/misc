package main

import (
	"io"
	"log"
	"net/http"
)

func main() {
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		log.Println("Receive request")
		io.WriteString(w, "from Origin Server\n")
	})

	log.Println("Origin Server Listen (:9001) ...")
	log.Fatalln(http.ListenAndServe(":9001", nil))
}
