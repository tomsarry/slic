CC=gcc
CFLAGS=-lm
GDBFLAGS=-g
DEPS=helpers.h tests.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

build: main.o helpers.o tests.o
	$(CC) -o main main.o helpers.o tests.o $(CFLAGS)

debug: main.o helpers.o tests.o
	$(CC) -o main main.o helpers.o tests.o $(CFLAGS) $(GDBFLAGS)
	gdb ./main

clean:
	rm *.o main