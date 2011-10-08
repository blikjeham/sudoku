/* const.h
 *
 * type declarations etc for sudoku
 */

#ifndef CONST_H
#define CONST_H 1

#define ONE   0x001
#define TWO   0x002
#define THREE 0x004
#define FOUR  0x008
#define FIVE  0x010
#define SIX   0x020
#define SEVEN 0x040
#define EIGHT 0x080
#define NINE  0x100
#define ALL   0x1ff

#define BLOCK 0
#define ROW 1
#define COL 2

struct square {
	unsigned int value:8;
	unsigned int possible:9;
	int initial;
	int left;
	int block;
	int row;
	int col;
} field[81];

/* Bruteforce backup */
struct square bf_backup[81];
int bruteforced;

struct single {
	int filled;
	int notfilled;
};

#ifdef HAVE_NCURSES
WINDOW *wfield;
WINDOW *wtext;
#else
void *wfield;
void *wtext;
#endif

#endif /* CONST_H */
