CC                = gcc
SOCKLIBDIR        = ../socketlib
SOCKLIB           = $(SOCKLIBDIR)/socketlib.a
CFLAGS            = -std=c99 -O2 -Wall -I$(SOCKLIBDIR)

all               : server client

server            : server.o app.o
	$(CC) -o $@ $^ $(SOCKLIB) -lcurses

client            : client.o app.o
	$(CC) -o $@ $^ $(SOCKLIB) -lcurses

server.o client.o : app.h

clean             :
	$(RM) server client *.o
