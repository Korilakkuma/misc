CC                      = gcc
SOCKLIBDIR              = ../socketlib
SOCKLIB                 = $(SOCKLIBDIR)/socketlib.a
CFLAGS                  = -Wall -O2 -std=c99 -I$(SOCKLIBDIR)
CFLAGS                  = -std=c99 -O2 -Wall -I$(SOCKLIBDIR)
OBJS                    = app.o appd.o app_inetd.o app_reader.o app_reader_from_service.o service.o session.o
DAEMONS                 = app appd app_inetd
CLIENTS                 = app_reader app_reader_from_service

all                     : $(DAEMONS) $(CLIENTS)

app                     : app.o service.o
	$(CC) -o $@ $^

appd                    : appd.o service.o
	$(CC) -o $@ $^

app_inetd               : app_inetd.o service.o
	$(CC) -o $@ $^

app_reader              : app_reader.o session.o
	$(CC) -o $@ $^ $(SOCKLIB)

app_reader_from_service : app_reader_from_service.o session.o
	$(CC) -o $@ $^ $(SOCKLIB)

$(OBJS)                 : app.h

clean                   :
	$(RM) $(DAEMONS) $(CLIENTS) $(OBJS)

v0                      : app app_reader
v1                      : appd app_reader
v2                      : app_inetd app_reader_from_service
