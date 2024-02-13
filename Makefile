CC=gcc
CFLAGS=-Wall -Wextra
TARGET=search

all: $(TARGET)

$(TARGET): kgulkotw_HW02.c
    $(CC) $(CFLAGS) kgulkotw_HW02.c -o $(TARGET)

clean:
    rm -f $(TARGET)

.PHONY: all clean
