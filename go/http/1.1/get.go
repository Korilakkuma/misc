package main

import (
	"crypto/tls"
	"crypto/x509"
	"io/ioutil"
	"log"
	"net/http"
	"net/http/httputil"
)

func main() {
	cert, err := ioutil.ReadFile("ca.crt")

	if err != nil {
		panic(err)
	}

	certPool := x509.NewCertPool()

	certPool.AppendCertsFromPEM(cert)

	tlsConfig := &tls.Config{RootCAs: certPool}
	// tlsConfig := &tls.Config{InsecureSkipVerify: true}

	tlsConfig.BuildNameToCertificate()

	client := &http.Client{
		Transport: &http.Transport{
			TLSClientConfig: tlsConfig,
		},
	}

	res, err := client.Get("https://localhost:18443")

	if err != nil {
		panic(err)
	}

	defer res.Body.Close()

	dump, err := httputil.DumpResponse(res, true)

	if err != nil {
		panic(err)
	}

	log.Println(string(dump))
}

/*
import (
	"log"
	"net/http"
	"net/http/httputil"
)

func main() {
	res, err := http.Get("https://localhost:18443")

	if err != nil {
		panic(err)
	}

	defer res.Body.Close()

	dump, err := httputil.DumpResponse(res, true)

	if err != nil {
		panic(err)
	}

	log.Println(string(dump))
}
*/
