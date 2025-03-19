CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -g

all: s21_grep


s21_grep: clean
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

clean:
	rm -rf s21_grep
	rm -rf *.o

