CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -Wpedantic -O2 -ansi
SRC = main.c src/*.c
OUT = server

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
