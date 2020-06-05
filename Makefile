all:
	@gcc main.c btree.c -o main.exe -lm -I -ansi.

run:
	@./main