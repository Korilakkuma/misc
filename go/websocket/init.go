package main

import (
	"fmt"
	"log"
	"net/http"
	"os"
	"strconv"

	"github.com/gorilla/websocket"
)

var upgrader = websocket.Upgrader{
	CheckOrigin: func(r *http.Request) bool {
		return true
	},
	ReadBufferSize:  2048,
	WriteBufferSize: 2048,
}

var websockets []websocket.Conn

func main() {
	if len(os.Args) != 2 {
		println("Usage:\n  $ go run . [port number]")
		os.Exit(1)
	}

	port, err := strconv.ParseUint(os.Args[1], 10, 16)
	if err != nil {
		println("Invalid port number !")
		os.Exit(1)
	}

	http.HandleFunc("/", handler)

	fmt.Printf("Listen ... (%d)\n\n", port)

	err = http.ListenAndServe(fmt.Sprintf(":%d", port), nil)
	if err != nil {
		panic(err)
	}
}

func handler(w http.ResponseWriter, r *http.Request) {
	conn, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Println(err)
		return
	}

	for {
		var data []byte

		_, data, err = conn.ReadMessage()
		if err != nil {
			log.Println(err)
		}

		for i, d := range data {
			fmt.Printf("Received[%d] %d\n", i, d)
		}

		// conn.WriteMessage(websocket.BinaryMessage, data)
	}
}
