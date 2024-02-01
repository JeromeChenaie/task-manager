# Compiler
CC = gcc

# Compilation options
CFLAGS =  -std=c99

# Source file names
SRCS = main.c task.c config.c gui.c

# Object file name
OBJS = $(SRCS:.c=.o)

# Header file names
HEADERS = task.h config.h gui.h

# Executable name
TARGET = task_manager

# ncurses library
LIBNCURSES = -lncurses

# Default rules
all: $(TARGET)

# Executable creation rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBNCURSES)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for cleaning object files
clean:
	rm -f $(OBJS)
