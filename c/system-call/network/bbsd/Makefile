CC         = gcc
SOCKLIBDIR = ../socketlib
SOCKLIB    = $(SOCKLIBDIR)/socketlib.a
CFLAGS     = -std=c99 -O2 -Wall -I$(SOCKLIBDIR)
OBJS       = app.o app_reader.o service.o session.o
DAEMONS    = app
CLIENTS    = app_reader

all        : $(DAEMONS) $(CLIENTS)

app        : app.o service.o
	$(CC) -o $@ $^

app_reader : app_reader.o session.o
	$(CC) -o $@ $^ $(SOCKLIB)

$(OBJS)    : app.h

clean      :
	$(RM) $(DAEMONS) $(CLIENTS) $(OBJS)

v0         : app app_reader
