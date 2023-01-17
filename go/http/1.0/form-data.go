package main

import (
	"bytes"
	"io"
	"log"
	"mime/multipart"
	"net/http"
	"os"
)

func main() {
	var buffer bytes.Buffer

	writer := multipart.NewWriter(&buffer)

	writer.WriteField("text", "text parameter")

	fileWriter, err := writer.CreateFormFile("file", "uploaded-file.png")

	if err != nil {
		panic(nil)
	}

	readFile, err := os.Open("./file.png")

	if err != nil {
		panic(nil)
	}

	defer readFile.Close()

	io.Copy(fileWriter, readFile)

	writer.Close()

	// "multipart/form-data: boundary=" + writer.Boundary()
	res, err := http.Post("http://localhost:18888", writer.FormDataContentType(), &buffer)

	if err != nil {
		panic(nil)
	}

	log.Println("Status: ", res.Status)
	log.Println("Headers: ", res.Header)
}
