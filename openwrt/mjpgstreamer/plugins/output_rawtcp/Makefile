###############################################################
#
# Purpose: Makefile for "M-JPEG Streamer"
# Author.: Tom Stoeveken (TST)
# Version: 0.3
# License: GPL
#
###############################################################

CC = ../../../mips-openwrt-linux-gcc

OTHER_HEADERS = ../../mjpg_streamer.h ../../utils.h ../output.h ../input.h

CFLAGS += -O2 -DLINUX -D_GNU_SOURCE -Wall -shared -fPIC
#CFLAGS += -DDEBUG
LFLAGS += -lpthread -ldl

all: output_rawtcp.so

clean:
	rm -f *.a *.o core *~ *.so *.lo

output_rawtcp.so: $(OTHER_HEADERS) output_rawtcp.c
	$(CC) $(CFLAGS) -o $@ output_rawtcp.c
