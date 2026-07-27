CC = gcc 
FLAGS = -Wall -Wextra -std=c11 -g 
LIBS = -lreadline

TARGET = csh

INCLUDE = args.c commands.c completer.c history.c path_builder.c pipe.c redirect.c utils.c
HEADERS = $(INCLUDE:.c=.h)
OBJECTS = $(INCLUDE:.c=.o) main.o

all: $(TARGET)

# $@ is refer to TARGET in this case
$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $(OBJECTS) $(LIBS)

%.o: %.c $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

# phony means not create file
.PHONY: all clean
