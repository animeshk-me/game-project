all: exp

exp: main_experiment.c
	gcc -lncurses -lpthread main_experiment.c -o exp

