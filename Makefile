CC=gcc
CFLAGS=-Wall -Wextra
TARGET=search

all: $(TARGET)

$(TARGET): search.c
	$(CC) $(CFLAGS) search.c -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
