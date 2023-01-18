package main

import (
	"bytes"
	"io"
	"log"
	"mime/multipart"
	"net/http"
	"net/textproto"
	"os"
)

func main() {
	part := make(textproto.MIMEHeader)
	part.Set("Content-Type", "image/png")
	part.Set("Content-Disposition", `form-data; name="file"; filename="file.png"`)

	var buffer bytes.Buffer

	writer := multipart.NewWriter(&buffer)

	writer.WriteField("text", "text parameter")

	fileWriter, err := writer.CreatePart(part)

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
