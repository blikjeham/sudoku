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
	int value;
	int possible;
	int left;
	int block;
	int row;
	int col;
} field[81];

struct single {
	int filled;
	int notfilled;
};

#endif /* CONST_H */
