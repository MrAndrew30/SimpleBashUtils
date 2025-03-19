CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11

all: s21_cat

tests:
	mkdir -p ../../data-samples/cat
	python3 main.py

s21_cat: clean
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

clean:
	rm -rf s21_cat
	rm -rf *.o

