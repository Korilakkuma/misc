package main

import (
	"fmt"
	"io/ioutil"
	"math/big"
	"net/http"
	"time"
)

var html []byte

func main() {
	var err error

	html, err = ioutil.ReadFile("index.html")

	if err != nil {
		panic(err)
	}

	http.HandleFunc("/", handlerHTML)
	http.HandleFunc("/sse", handlerSSE)

	fmt.Println("Listen (18888) ...")

	err = http.ListenAndServe(":18888", nil)

	fmt.Println(err)
}

func handlerHTML(w http.ResponseWriter, r *http.Request) {
	w.Header().Add("Content-Type", "text/html")
	w.Write(html)
}

func handlerSSE(w http.ResponseWriter, r *http.Request) {
	flusher, ok := w.(http.Flusher)

	if !ok {
		http.Error(w, "Streaming unsupported !", http.StatusInternalServerError)
		return
	}

	ctx := r.Context()

	w.Header().Set("Content-Type", "text/event-stream")
	w.Header().Set("Cache-Control", "no-cache")
	w.Header().Set("Connection", "keep-alive")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	var num int64 = 1

	for id := 1; id <= 100; id++ {
		select {
		case <-ctx.Done():
			fmt.Println("Connection closed from client")
			return
		default:
			// do nothing
		}

		for {
			num++

			if big.NewInt(num).ProbablyPrime(20) {
				fmt.Println(num)

				// If use `event` field
				// fmt.Fprintf(w, "id: %d\nevent: ping\n", id)

				fmt.Fprintf(w, "data: { \"id\": %d, \"number\": %d }\n\n", id, num)
				flusher.Flush()
				time.Sleep(time.Second)
				break
			}

			time.Sleep(time.Second)
		}

		fmt.Println("Connection closed from server")
	}
}
