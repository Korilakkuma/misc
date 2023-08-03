package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"net/http/httputil"
)

func main() {
	var httpServer http.Server

	http.HandleFunc("/", handler)
	http.HandleFunc("/basic", digestHandler)
	log.Println("Listen 18888 ...")
	httpServer.Addr = ":18888"
	log.Println(httpServer.ListenAndServe())
}

func handler(w http.ResponseWriter, r *http.Request) {
	dump, err := httputil.DumpRequest(r, true)

	if err != nil {
		http.Error(w, fmt.Sprint(err), http.StatusInternalServerError)
		return
	}

	fmt.Println(string(dump))
	fmt.Fprintf(w, "<html><body>Basic Authentication</body></html>\n")
}

func digestHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Printf("URL: %s\n", r.URL.String())
	fmt.Printf("Query: %v\n", r.URL.Query())
	fmt.Printf("Proto: %s\n", r.Proto)
	fmt.Printf("Method: %s\n", r.Method)
	fmt.Printf("Header: %v\n", r.Header)

	defer r.Body.Close()

	body, _ := ioutil.ReadAll(r.Body)

	fmt.Printf("--body--\n%s\n", string(body))

	if _, ok := r.Header["Authorization"]; !ok {
		w.Header().Add("WWW-Authenticate", `Basic realm="realm"`)
		w.WriteHeader(http.StatusUnauthorized)
	} else {
		if r.Header.Get("Authorization") == "Basic a29yaWxha2t1bWE6a29yaWxhOW1h" {
			fmt.Fprintf(w, "<html><body>Authorized Page</body></html>\n")
		} else {
			w.Header().Add("WWW-Authenticate", `Basic realm="realm"`)
			w.WriteHeader(http.StatusUnauthorized)
		}
	}
}
