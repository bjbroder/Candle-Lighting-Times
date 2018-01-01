9C = gcc

CSAPP_INC = ../../../include
CSAPP_SRC = ../../../src

CFLAGS = -Wall -O2 -I $(CSAPP_INC) -I .
LDLIBS = -lpthread



all: doingSomething


clean:
	rm doingSomething

doingSomething: doingSomething.c
	$(CC) $(CFLAGS) doingSomething.c $(CSAPP_SRC)/csapp.o $(LDLIBS) -o doingSomething
