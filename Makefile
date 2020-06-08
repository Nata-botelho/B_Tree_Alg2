all:
	@gcc main.c btree.c -o main.exe -g -Wall -ansi -lm -I.

gen:
	@gcc name_generator.c -o name_generator.exe -Wall -lm -I.

genrun:
	@./name_generator.exe

run:
	@./main