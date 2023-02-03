package main

import (
	"log"
	"net"
	"net/http"
	"net/rpc"
	"net/rpc/jsonrpc"
)

type Calculator int

type Args struct {
	A int
	B int
}

func main() {
	calculator := new(Calculator)
	server := rpc.NewServer()

	server.Register(calculator)
	http.Handle(rpc.DefaultRPCPath, server)
	log.Println("Listen (:18888) ...")

	listener, err := net.Listen("tcp", ":18888")

	if err != nil {
		panic(err)
	}

	for {
		conn, err := listener.Accept()

		if err != nil {
			panic(err)
		}

		go server.ServeCodec(jsonrpc.NewServerCodec(conn))
	}
}

func (c *Calculator) RemoteProcedure(args Args, result *int) error {
	log.Printf("Remote Process: %d, %d\n", args.A, args.B)
	*result = args.A * args.B
	return nil
}
