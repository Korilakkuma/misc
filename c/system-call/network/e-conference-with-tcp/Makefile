CC          = gcc
SOCKLIBDIR  = ../socketlib
SOCKLIB     = $(SOCKLIBDIR)/socketlib.a
CFLAGS      = -std=c99 -O2 -Wall -I$(SOCKLIBDIR)
SERVER_OBJS = server.o app_server.o
CLIENT_OBJS = client.o app_client.o

all         : server client

server      : $(SERVER_OBJS)
	$(CC) -o $@ $^ $(SOCKLIB) -lcurses

client      : $(CLIENT_OBJS)
	$(CC) -o $@ $^ $(SOCKLIB) -lcurses

server.o    : app_server.h
client.o    : app_client.h

clean       :
	$(RM) server client $(SERVER_OBJS) $(CLIENT_OBJS)
