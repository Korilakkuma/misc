package main

import (
	"bytes"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"net/http/httputil"
	"strconv"
)

func main() {
	director := func(request *http.Request) {
		request.URL.Scheme = "http"
		request.URL.Host = ":9001"
	}

	modifier := func(res *http.Response) error {
		body, err := ioutil.ReadAll(res.Body)

		if err != nil {
			return fmt.Errorf("Reading body error: %w", err)
		}

		newBody := bytes.NewBuffer(body)
		newBody.WriteString("via Proxy")

		res.Body = ioutil.NopCloser(newBody)
		res.Header.Set("Content-Length", strconv.Itoa(newBody.Len()))

		return nil
	}

	rp := &httputil.ReverseProxy{
		Director:       director,
		ModifyResponse: modifier,
	}

	server := http.Server{
		Addr:    "127.0.0.1:9000",
		Handler: rp,
	}

	log.Println("Listen (:9000) ...")
	log.Fatalln(server.ListenAndServe())
}
