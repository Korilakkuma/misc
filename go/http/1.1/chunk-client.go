package main

import (
	"bufio"
	"io"
	"log"
	"net"
	"net/http"
	"strconv"
	"time"
)

func main() {
	dialer := net.Dialer{
		Timeout:   30 * time.Second,
		KeepAlive: 30 * time.Second,
	}

	conn, err := dialer.Dial("tcp", "localhost:18888")

	if err != nil {
		panic(err)
	}

	defer conn.Close()

	req, err := http.NewRequest("GET", "http://localhost:18888/chunked", nil)
	err = req.Write(conn)

	if err != nil {
		panic(err)
	}

	reader := bufio.NewReader(conn)
	res, err := http.ReadResponse(reader, req)

	if err != nil {
		panic(err)
	}

	if res.TransferEncoding[0] != "chunked" {
		panic("Wrong Transfer-Encoding header")
	}

	for {
		sizeAsString, err := reader.ReadBytes('\n')

		if err == io.EOF {
			panic(err)
		}

		size, err := strconv.ParseInt(string(sizeAsString[:len(sizeAsString)-2]), 16, 64)

		if err != nil {
			panic(err)
		}

		if size == 0 {
			break
		}

		line := make([]byte, int(size))

		reader.Read(line)
		reader.Discard(2)

		log.Println(" ", string(line))
	}
}

/*
import (
	"bufio"
	"bytes"
	"io"
	"log"
	"net/http"
)

func main() {
	res, err := http.Get("http://localhost:18888/chunked")

	if err != nil {
		panic(err)
	}

	defer res.Body.Close()

	reader := bufio.NewReader(res.Body)

	for {
		line, err := reader.ReadBytes('\n')

		if err == io.EOF {
			break
		}

		log.Println(bytes.TrimSpace(line))
	}
}
*/
