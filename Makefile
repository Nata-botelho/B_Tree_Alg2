gen:
	@gcc name_generator.c -o name_generator.exe -Wall -lm -I.

genrun:
	@./name_generator

all:
	@gcc main.c btree.c -o main.exe -Wall -lm -I -ansi.

run:
	@./main