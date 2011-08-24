#include <stdlib.h>
#include "s_util.h"
#include "const.h"

extern struct single row[9];
extern struct single col[9];
extern struct single block[9];

void printfield(void)
{
	int i;
	printf("+-------+-------+-------+\n| ");
	for (i=0; i<81; i++) {
		printf("%03d ", field[i].possible);
		if ((i % 81) != 80) {
			if ((i % 3) == 2)
				printf("| ");
			if ((i % 9) == 8)
				printf("\n| ");
		} else {
			printf("|\n");
		}
	       
	}
	printf("+-------+-------+-------+\n");
}

int readfield(FILE *fd)
{
	int i;
	extern struct square field[81];

	for (i=0; i<81; i++) {
		field[i].value = (fgetc(fd) & ~0x30);
	}
	return(0);
}

void fill_all(void)
{
	int i;
	for (i=0; i<81; i++) {
		if (field[i].value == 0) {
			field[i].possible = ALL;
			field[i].left = 9;
		}
	}
	for (i=0; i<9; i++) {
		row[i].filled = 0;
		row[i].notfilled = ALL;
		col[i].filled = 0;
		col[i].notfilled = ALL;
		block[i].filled = 0;
		block[i].notfilled = ALL;
	}
}

int i_to_block(int square)
{
	return( ((i_to_row(square)/3)*3) +
		(i_to_col(square)/3)
		);

}

int i_to_row(int square)
{
	return(square / 9);
}

int i_to_col(int square)
{
	return(square % 9);
}

int vtom(int value)
{
	switch(value) {
	case 1:
		return(ONE);
		break;
	case 2:
		return(TWO);
		break;
	case 3:
		return(THREE);
		break;
	case 4:
		return(FOUR);
		break;
	case 5:
		return(FIVE);
		break;
	case 6:
		return(SIX);
		break;
	case 7:
		return(SEVEN);
		break;
	case 8:
		return(EIGHT);
		break;
	case 9:
		return(NINE);
		break;
	default:
		return(0);
	}
}

int mtov(int mask)
{
	switch(mask) {
	case ONE:
		return(1);
		break;
	case TWO:
		return(2);
		break;
	case THREE:
		return(3);
		break;
	case FOUR:
		return(4);
		break;
	case FIVE:
		return(5);
		break;
	case SIX:
		return(6);
		break;
	case SEVEN:
		return(7);
		break;
	case EIGHT:
		return(8);
		break;
	case NINE:
		return(9);
		break;
	default:
		return(0);
	}
}

void fill_block(int blocknum, int value)
{
	/* remove possibilities */
	int i;
	extern struct single block[9];
	for (i=0; i<81; i++) {
		if ((i_to_block(i) == blocknum)
		    && (field[i].value == 0) 
		    && (field[i].possible & vtom(value))
		    ) {
			field[i].possible &= ~vtom(value);
			field[i].left--;
		}
	}
			
	/* set filled */
	block[blocknum].filled |= vtom(value);
}

void fill_row(int rownum, int value)
{
	/* remove possibilities */
	int i;
	extern struct single row[9];
	for (i=0; i<81; i++) {
		if ((i_to_row(i) == rownum)
		    && (field[i].value == 0)
		    && (field[i].possible & vtom(value))
		    ) {
			field[i].possible &= ~vtom(value);
			field[i].left--;
		}
	}

	/* set filled */
	row[rownum].filled |= vtom(value);
}

void fill_col(int colnum, int value)
{
	/* remove possibilities */
	int i;
	extern struct single col[9];
	for (i=0; i<81; i++) {
		if ((i_to_col(i) == colnum)
		    && (field[i].value == 0)
		    && (field[i].possible & vtom(value))
		    ) {
			field[i].possible &= ~vtom(value);
			field[i].left--;
		}
	}

	/* set filled */
	col[colnum].filled |= vtom(value);
}


/* is the number still a possibility? */
int get_value(int possib)
{
	if (possib & ONE)
		return(1);
	if (possib & TWO)
		return(2);
	if (possib & THREE)
		return(3);
	if (possib & FOUR)
		return(4);
	if (possib & FIVE)
		return(5);
	if (possib & SIX)
		return(6);
	if (possib & SEVEN)
		return(7);
	if (possib & EIGHT)
		return(8);
	if (possib & NINE)
		return(9);
	return 0;
}
