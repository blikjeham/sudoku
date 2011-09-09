CFLAGS += -ggdb 
CFLAGS += -Wall
all: sudoku

clean: sudoku
	rm sudoku

sudoku: main.c s_util.c
	gcc $(CFLAGS) -o sudoku main.c s_util.c


test: test.c s_util.c
	gcc $(CFLAGS) -o test test.c s_util.c