CC          = gcc
CFLAGS      = -std=c99 -O2 -Wall
OBJS        = setup_server.o setup_client.o trim_newline.o
AR          = ar -qc

all         : socketlib.a

socketlib.a : $(OBJS)
	$(RM) $@
	$(AR) $@ $^

$(OBJS)     : socketlib.h

clean       :
	$(RM) *.o socketlib.a
