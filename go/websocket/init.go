package main

import (
	"fmt"
	"golang.org/x/net/websocket"
	"net/http"
	"os"
	"strconv"
)

func main() {
	if len(os.Args) != 2 {
		println("Require port number !")
		os.Exit(1)
	}

	port, err := strconv.ParseUint(os.Args[1], 10, 16)

	if err != nil {
		println("Invalid port number !")
		os.Exit(1)
	}

	var websockets []websocket.Conn

	http.Handle("/", websocket.Handler(func(ws *websocket.Conn) {
		var in []byte

		websockets = append(websockets, *ws)

		for {
			if err = websocket.Message.Receive(ws, &in); err != nil {
				println(err)
				break
			}

			for _, v := range websockets {
				if v != *ws {
					websocket.Message.Send(&v, in)
				}
			}
		}
	}))

	err = http.ListenAndServe(fmt.Sprintf(":%d", port), nil)

	if err != nil {
		panic(err)
	}
}
