CC     = gcc
CFLAGS = -std=c99 -O2 -Wall

all    : server client

server : server.c
	$(CC) $(CFLAGS) -o $@ $<

client : client.c
	$(CC) $(CFLAGS) -o $@ $<

clean  :
	$(RM) server client
