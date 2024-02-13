CC=gcc
CFLAGS=-Wall -g
TARGET=kgulkotw_HW02

# Default build target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): kgulkotw_HW02.c
	$(CC) $(CFLAGS) -o $(TARGET) kgulkotw_HW02.c

# Clean the built files
clean:
	rm -f $(TARGET)

.PHONY: all clean
