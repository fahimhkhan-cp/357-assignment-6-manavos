CC = gcc
CFLAGS = -Wall -std=c99 -pedantic

SRC = asgn6.c
TARGET = program

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm *.o $(MAIN) core

# Phony targets
.PHONY: all clean