CC=gcc
CFLAGS=-lm
GDBFLAGS=-g
OBJFILES=main.o helpers.o colorHelpers.o ioHelpers.o tests.o
TARGET=main

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -o $(TARGET) $(OBJFILES) $(CFLAGS)

debug: $(OBJFILES)
	$(CC) -o $(TARGET) $(OBJFILES) $(CFLAGS) $(GDBFLAGS)
	gdb ./$(TARGET)

clean:
	rm *.o $(TARGET)