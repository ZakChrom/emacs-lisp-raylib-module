CC=gcc
CFLAGS=-Wall -Wextra -shared -fPIC
LDFLAGS=-lraylib -lm -lpthread

.PHONY: all clean

all: rlmodule.so

clean:
	rm rlmodule.so

rlmodule.so: rlmodule.c
	gcc rlmodule.c $(CFLAGS) $(LDFLAGS) -o rlmodule.so