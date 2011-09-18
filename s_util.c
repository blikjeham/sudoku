#include "config.h"

#ifdef HAVE_NCURSES
#include <ncurses.h>
#endif /* HAVE_NCURSES */

#include <stdlib.h>
#include <strings.h>
#include "s_util.h"
#include "const.h"

extern struct single row[9];
extern struct single col[9];
extern struct single block[9];

void winprintf(WINDOW *wfield, char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
#ifdef HAVE_NCURSES
	char buffer[1024];
	vsnprintf(buffer, 1023, fmt, va);
	waddstr(wfield, buffer);
	wrefresh(wfield);
#else
	vprintf(fmt,va);
#endif /* HAVE_NCURSES */
	va_end(va);
}

void check_filled(void)
{
	int i;
	int brc;

	for (i=0; i<81; i++) {
		if (field[i].value > 0) {
			for (brc=0; brc<3; brc++) {
				fill_brc(brc, i_to_brc(brc, i), field[i].value);
			}
		}
	}
}

void check_only(void)
{
	int i;

	for (i=0; i<81; i++) {
		if ((field[i].value == 0) && (field[i].left == 1)) {
			field[i].value = mtov(field[i].possible);
			field[i].left = 0;
			field[i].possible = 0;
			check_filled();
		}
	}
}

void check_single(void)
{
	int i,num;
	int brc;

	for (num=1; num<10; num++) {
		for (i=0; i<9; i++) {
			for (brc=0; brc<3; brc++) {
				if (check_num(brc, i, vtom(num)) == 1) {
					set_num(brc, i, num);
				}
			}
		}
	}
}

void check_double(void)
{
	int num;
	int i;
	int a_brc[3] = {0, 0, 0};

	for (num=1; num<10; num++) {
		for (i=0; i<9; i++) {
			bzero(a_brc, sizeof(int)*3);
			if (check_num_brc(BLOCK, a_brc, i, vtom(num)) == 2) {
				fill_brc_ex(BLOCK, i, a_brc, vtom(num));
			}
			if (check_num_brc(ROW, a_brc, i, vtom(num)) == 2) {
				fill_brc_ex(ROW, i, a_brc, vtom(num));
			}
			if (check_num_brc(COL, a_brc, i, vtom(num)) == 2) {
				fill_brc_ex(COL, i, a_brc, vtom(num));
			}
		}
	}
}

void check_double_value(void)
{
	int i;
	int mask;
	int a_brc[3] = {0, 0, 0};

	for (mask=ONE; mask<=ALL; mask++) {
		for (i=0; i<9; i++) {
			bzero(a_brc, sizeof(int)*3);
			if (check_num_brc(BLOCK, a_brc, i, mask) == mtop(mask)) {
				fill_brc_double(BLOCK, i, a_brc, mask);
			}
			if (check_num_brc(ROW, a_brc, i, mask) == mtop(mask)) {
				fill_brc_double(ROW, i, a_brc, mask);
			}
			if (check_num_brc(ROW, a_brc, i, mask) == mtop(mask)) {
				fill_brc_double(ROW, i, a_brc, mask);
			}
		}
		check_single();
	}
}

int get_left(void)
{
	int i;
	int left=0;
	for (i=0; i<81; i++) {
		left += field[i].left;
	}
	return(left);
}

void printfield(WINDOW *wfield, int possible)
{
	int i;
	wmove(wfield, 0,0);
	werase(wfield);
	wrefresh(wfield);
	if (possible) {
		winprintf(wfield, "    0   1   2     3   4   5     6   7   8\n\r");
		winprintf(wfield, " +-------------+-------------+-------------+\n\r0| ");
	}
	else {
		winprintf(wfield, "   0 1 2   3 4 5   6 7 8\n\r");
		winprintf(wfield, " +-------+-------+-------+\n\r0| ");
	}
	for (i=0; i<81; i++) {
		if (field[i].value == 0) {
			if (possible)
				winprintf(wfield, "%03d ", field[i].possible);
			else
				winprintf(wfield, ". ");
		} else {
			if (possible)
				winprintf(wfield, ".%d. ", field[i].value);
			else
				winprintf(wfield, "%d ", field[i].value);
		}
		if ((i % 81) != 80) {
			if ((i % 3) == 2)
				winprintf(wfield, "| ");
			if ((i % 27) == 26) {
				if (possible)
					winprintf(wfield, "\n\r +-------------+-------------+-------------+");
				else
					winprintf(wfield, "\n\r +-------+-------+-------+");
			}
			if ((i % 9) == 8)
				winprintf(wfield, "\n\r%d| ", (i/9)+1);
		} else {
			winprintf(wfield, "|\n\r");
		}
	       
	}
	if (possible)
		winprintf(wfield, " +-------------+-------------+-------------+\n\r\n\r");
	else
		winprintf(wfield, " +-------+-------+-------+\n\r\n\r");
	if (possible) {
		for (i=1; i<10; i++) {
			winprintf(wfield, "%d=%d; ", i, vtom(i));
			if (i % 5 == 0)
				winprintf(wfield, "\n\r");
		}
		winprintf(wfield, "\n\r");
	}
	wrefresh(wfield);
}

int readfield(FILE *fd)
{
	int i;
	char number;
	extern struct square field[81];

	for (i=0; i<81; i++) {
		number = fgetc(fd);
		if ((number < 0x30) || (number > 0x3a)) {
			/* the character read is actually not a number
			   so skip it and continue. */
			i--;
			continue;
		}
		field[i].value = (number & ~0x30);
	}
	return(0);
}

void fill_all(void)
{
	int i;
	for (i=0; i<81; i++) {
		field[i].block = i_to_brc(BLOCK, i);
		field[i].row = i_to_brc(ROW, i);
		field[i].col = i_to_brc(COL, i);
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

int check_num_brc(int brc, int *x_brc, int where, int mask)
{
	int i;
	int num=0;
	for (i=0; i<81; i++) {
		if ( (brc == BLOCK)
		     && (i_to_brc(BLOCK, i) == where)
		     && (field[i].value == 0)
		     && ((field[i].possible & mask) == mask)
		     ) {
			num++;
			x_brc[ROW] |= vtom(i_to_brc(ROW, i)+1);
			x_brc[COL] |= vtom(i_to_brc(COL, i)+1);
		}
		if ( (brc == ROW)
		     && (i_to_brc(ROW, i) == where)
		     && (field[i].value == 0)
		     && ((field[i].possible & mask) == mask)
		     ) {
			num++;
			x_brc[BLOCK] |= vtom(i_to_brc(BLOCK, i)+1);
			x_brc[COL] |= vtom(i_to_brc(COL, i)+1);
		}
		if ( (brc == COL)
		     && (i_to_brc(COL, i) == where)
		     && (field[i].value == 0)
		     && ((field[i].possible & mask) == mask)
		     ) {
			num++;
			x_brc[BLOCK] |= vtom(i_to_brc(BLOCK, i)+1);
			x_brc[ROW] |= vtom(i_to_brc(ROW, i)+1);
		}
	}

	return(num);
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

	for (i=0; i<81; i++) {
		if ((i_to_brc(brc, i) == where)
		    && (field[i].value == 0)
		    && (field[i].possible & vtom(value))
		    ) {
			field[i].value = value;
			field[i].possible = 0;
			field[i].left = 0;
			
			/* Check again for the impossibilities */
			check_filled();
		}
	}
}

void fill_brc_ex(int brc, int where, int *excl, int value)
{
	int i;
	extern struct single block[9];
	extern struct single row[9];
	extern struct single col[9];

	for (i=0; i<81; i++) {
		if (brc == BLOCK) {
			if ( (i_to_brc(BLOCK, i) != where)
			     && ((excl[ROW]) == (vtom(i_to_brc(ROW, i)+1))
				 || (excl[COL]) == (vtom(i_to_brc(COL, i)+1)))
			     && (field[i].value == 0)
			     && (field[i].possible & value)
			     ) {
				field[i].possible &= ~(value);
				field[i].left--;
			}
		} else if (brc == ROW) {
			if ( (i_to_brc(ROW, i) != where)
			     && ((excl[BLOCK]) == (vtom(i_to_brc(BLOCK, i)+1))
				 || (excl[COL]) == (vtom(i_to_brc(COL, i)+1)))
			     && (field[i].value == 0)
			     && (field[i].possible & value)
			     ) {
				field[i].possible &= ~(value);
				field[i].left--;
			}
		} else if (brc == COL) {
			if ( (i_to_brc(COL, i) != where)
			     && ((excl[BLOCK]) == (vtom(i_to_brc(BLOCK, i)+1))
				 || (excl[ROW]) == (vtom(i_to_brc(ROW, i)+1)))
			     && (field[i].value == 0)
			     && (field[i].possible & value)
			     ) {
				field[i].possible &= ~(value);
				field[i].left--;
			}
		}
	}
}

void fill_brc_double(int brc, int where, int *excl, int value)
{
	int i;
	int removed;
	extern struct single block[9];
	extern struct single row[9];
	extern struct single col[9];

	for (i=0; i<81; i++) {
		removed=0;
		if (brc == BLOCK) {
			if ( (i_to_brc(BLOCK, i) == where)
			     && (!((excl[ROW]) & (vtom(i_to_brc(ROW, i)+1)))
				 || (!((excl[COL]) & (vtom(i_to_brc(COL, i)+1)))))
			     && (field[i].value == 0)
			     && (field[i].possible & value)
			     ) {
				removed = field[i].possible & value;
				field[i].possible &= ~(value);
				field[i].left = field[i].left - mtop(removed);
			}
		} else if (brc == ROW) {
			if ( (i_to_brc(ROW, i) == where)
			     && (!((excl[BLOCK]) & (vtom(i_to_brc(BLOCK, i)+1)))
				 || (!((excl[COL]) & (vtom(i_to_brc(COL, i)+1)))))
			     && (field[i].value == 0)
			     && (field[i].possible & value)
			     ) {
				removed = field[i].possible & value;
				field[i].possible &= ~(value);
				field[i].left = field[i].left - mtop(removed);
			}
		} else if (brc == COL) {
			if ( (i_to_brc(COL, i) == where)
			     && (!((excl[BLOCK]) & (vtom(i_to_brc(BLOCK, i)+1)))
				 || (!((excl[ROW]) & (vtom(i_to_brc(ROW, i)+1)))))
			     && (field[i].value == 0)
			     && (field[i].possible & value)
			     ) {
				removed = field[i].possible & value;
				field[i].possible &= ~(value);
				field[i].left = field[i].left - mtop(removed);
			}
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

int mtop(int mask)
{
	int i;
	int count=0;
	for (i=1; i<10; i++) {
		if (mask & vtom(i))
			count++;
	}
	return count;
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
