#include <stdlib.h>
#include "s_util.h"
#include "const.h"

extern struct single row[9];
extern struct single col[9];
extern struct single block[9];

void printfield(void)
{
	int i;
	printf("   0 1 2   3 4 5   6 7 8\n");
	printf(" +-------+-------+-------+\n0| ");
	for (i=0; i<81; i++) {
		if (field[i].value == 0)
			printf(". ");
		else
			printf("%d ", field[i].value);
		if ((i % 81) != 80) {
			if ((i % 3) == 2)
				printf("| ");
			if ((i % 27) == 26)
				printf("\n +-------+-------+-------+");
			if ((i % 9) == 8)
				printf("\n%d| ", (i/9)+1);
		} else {
			printf("|\n");
		}
	       
	}
	printf(" +-------+-------+-------+\n\n");
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

int i_to_brc(int brc, int where)
{
	switch(brc) {
	case BLOCK:
		return( ((i_to_brc(ROW, where)/3)*3) +
			(i_to_brc(COL, where)/3)
			);
	case ROW:
		return(where / 9);
	case COL:
		return(where % 9);
	}
	return(0);
}

int check_num(int brc, int where, int mask)
{
	int i;
	int num=0;

	for (i=0; i<81; i++) {
		if ( (i_to_brc(brc, i) == where)
		     && (field[i].value == 0)
		     && (field[i].possible & mask)
		     ) {
			num++;
		}
	}
	return(num);
}

void set_num(int brc, int where, int value)
{
	int i;

	printf("brc: %d, where: %d, value: %d\n", brc, where, value);
	for (i=0; i<81; i++) {
		if ((i_to_brc(brc, i) == where)
		    && (field[i].value == 0)
		    && (field[i].possible & vtom(value))
		    ) {
			field[i].value = value;
			field[i].possible = 0x0;
			field[i].left = 0;
		}
	}
}

void fill_brc(int brc, int where, int value)
{
	int i;

	/* load the appropriate struct single */
	extern struct single block[9];
	extern struct single row[9];
	extern struct single col[9];

	/* Remove the possibility of value for each field in the block, row, or column */
	for (i=0; i<81; i++) {
		if ((i_to_brc(brc, i) == where)
		    && (field[i].value == 0) 
		    && (field[i].possible & vtom(value))
		    ) {
			field[i].possible &= ~vtom(value);
			field[i].left--;
		}
	}
			
	/* set filled for this value */
	switch(brc) {
	case BLOCK:
		block[where].filled |= vtom(value);
		break;
	case ROW:
		row[where].filled |= vtom(value);
		break;
	case COL:
		col[where].filled |= vtom(value);
		break;
	}
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
