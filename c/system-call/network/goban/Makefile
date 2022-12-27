CC                = gcc
SOCKLIBDIR        = ../socketlib
SOCKLIB           = $(SOCKLIBDIR)/socketlib.a
CFLAGS            = -std=c99 -O2 -Wall -I$(SOCKLIBDIR)

all               : server client

server            : server.o app.o
	$(CC) -o $@ $^ $(SOCKLIB)

client            : client.o app.o
	$(CC) -o $@ $^ $(SOCKLIB)

server.o client.o : app.h

clean             :
	$(RM) server client *.o
