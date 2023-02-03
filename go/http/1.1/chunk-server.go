package main

import (
	"fmt"
	"log"
	"net/http"
	"time"
)

func main() {
	var httpServer http.Server

	http.HandleFunc("/chunked", chunkedHandler)
	log.Println("Listen 18888 ...")
	httpServer.Addr = ":18888"
	log.Println(httpServer.ListenAndServe())
}

func chunkedHandler(w http.ResponseWriter, r *http.Request) {
	flusher, ok := w.(http.Flusher)

	if !ok {
		panic("Error: http.Flusher")
	}

	for i := 1; i <= 10; i++ {
		fmt.Fprintf(w, "Chunk %d\n", i)
		flusher.Flush()
		time.Sleep(500 * time.Millisecond)
	}

	flusher.Flush()
}
