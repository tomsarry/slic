CC=gcc
CFLAGS=-lm
DEPS=helpers.h tests.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

build: main.o helpers.o tests.o
	$(CC) -o main main.o helpers.o tests.o $(CFLAGS)

clean:
	rm *.o main