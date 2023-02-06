package main

import (
	"fmt"
	"log"
	"net/http"
	"net/http/httputil"
	"net/url"
	"time"
)

type RetryTransport struct {
}

func main() {
	target, _ := url.Parse("http://127.0.0.1:9001")

	rp := httputil.NewSingleHostReverseProxy(target)

	rp.Transport = &RetryTransport{}

	server := http.Server{
		Addr:    "127.0.0.1:9000",
		Handler: rp,
	}

	log.Println("Listen (:9000) ...")
	log.Fatalln(server.ListenAndServe())
}

func (RetryTransport) RoundTrip(req *http.Request) (res *http.Response, err error) {
	for i := 0; i < 3; i++ {
		res, err := http.DefaultTransport.RoundTrip(req)

		if err != nil {
			log.Println("fail")
			time.Sleep(time.Second)
			continue
		}

		return res, nil
	}

	return nil, fmt.Errorf("failed to request to %s", req.URL.String())
}
