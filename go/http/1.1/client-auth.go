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
	// cert, err := tls.LoadX509KeyPair("client.crt", "client.key")

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

	// client := &http.Client{
	// 	Transport: &http.Transport{
	// 		TLSClientConfig: &tls.Config{
	// 			Certificates: []tls.Certificate(cert),
	// 		},
	// 	},
	// }

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
