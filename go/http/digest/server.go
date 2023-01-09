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
	http.HandleFunc("/digest", digestHandler)
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
	fmt.Fprintf(w, "<html><body>Digest Access Authentication</body></html>\n")
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
		w.Header().Add("WWW-Authenticate", `Digest realm="Secret Zone", nonce="TgLc25U2BQA=f519a2780473e18e6587be702c2e67fe2b04afd", algorithm=MD5, qop="auth"`)
		w.WriteHeader(http.StatusUnauthorized)
	} else {
		fmt.Fprintf(w, "<html><body>Authorized Page</body></html>\n")
	}
}
