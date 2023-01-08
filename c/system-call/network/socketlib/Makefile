CC          = gcc
CFLAGS      = -std=c99 -O2 -Wall
OBJS        = setup_server.o setup_client.o setup_multiple_server.o trim_newline.o
OBJSV6      = setup_server_v6.o setup_client_v6.o setup_multiple_server_v6.o trim_newline.o
AR          = ar -qc

all         : socketlib.a

socketlib.a : $(OBJS)
	$(RM) $@
	$(AR) $@ $^

socketlibv6.a : $(OBJSV6)
	$(RM) $@
	$(AR) $@ $^

$(OBJS)     : socketlib.h
$(OBJSV6)   : socketlib.h

clean       :
	$(RM) *.o socketlib.a socketlibv6.a
