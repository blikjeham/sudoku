#include "config.h"

#include <stdlib.h>
#include <strings.h>
#include "ui.h"
#include "s_util.h"
#include "bf_util.h"
#include "const.h"

extern struct single row[9];
extern struct single col[9];
extern struct single block[9];

/* Convert mask to number of possibilities */
static int mtop(int mask)
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
	return(1 << (value-1));
}

int mtov(int mask)
{
	int i;
	for (i = 0; i < 10; i++) {
		if ((mask >> i) == 0)
			return(i);
	}
	return(-1);
}

/* Check for the occurance of a mask in the brc
 * 'normal' (i.e. loose) function allows for other
 * possibilities to be present.
 * exact only matches the mask when no other possibilities
 * are present.
 */
static int check_num_brc(int brc, int *x_brc, int where, int mask)
{
	int i;
	int count=0;

	for (i=0; i<81; i++) {
		if ( (i_to_brc(brc, i) == where)
		     && (field[i].value == 0)
		     && ((field[i].possible & mask) == mask)
		     ) {
			count++;
			x_brc[BLOCK] |= vtom(i_to_brc(BLOCK, i)+1);
			x_brc[ROW] |= vtom(i_to_brc(ROW, i)+1);
			x_brc[COL] |= vtom(i_to_brc(COL, i)+1);
		}
	}
	return(count);
}

static int check_num_brc_exact(int brc, int *x_brc, int where, int mask)
{
	int i;
	int count=0;

	for (i=0; i<81; i++) {
		if ( (brc == BLOCK)
		     && (i_to_brc(BLOCK, i) == where)
		     && (field[i].value == 0)
		     && (field[i].possible == mask)
		     ) {
			count++;
			x_brc[ROW] |= vtom(i_to_brc(ROW, i)+1);
			x_brc[COL] |= vtom(i_to_brc(COL, i)+1);
		}
		if ( (brc == ROW)
		     && (i_to_brc(ROW, i) == where)
		     && (field[i].value == 0)
		     && (field[i].possible == mask)
		     ) {
			count++;
			x_brc[BLOCK] |= vtom(i_to_brc(BLOCK, i)+1);
			x_brc[COL] |= vtom(i_to_brc(COL, i)+1);
		}
		if ( (brc == COL)
		     && (i_to_brc(COL, i) == where)
		     && (field[i].value == 0)
		     && (field[i].possible == mask)
		     ) {
			count++;
			x_brc[BLOCK] |= vtom(i_to_brc(BLOCK, i)+1);
			x_brc[ROW] |= vtom(i_to_brc(ROW, i)+1);
		}
	}
	return(count);
}

static int check_num_brc_loose(int brc, int *x_brc, int where, int mask)
{
	int i;
	int count=0;
	int extracount=0;

	for (i=0; i<81; i++) {
		if ( (i_to_brc(brc, i) == where)
		     && (field[i].value == 0)
		     && ((field[i].possible & mask) == mask)
		     ) {
			count++;
			x_brc[BLOCK] |= vtom(i_to_brc(BLOCK, i)+1);
			x_brc[ROW] |= vtom(i_to_brc(ROW, i)+1);
			x_brc[COL] |= vtom(i_to_brc(COL, i)+1);
		} else if ( (i_to_brc(brc, i) == where)
			    && (field[i].value == 0)
			    && (field[i].possible & mask)
			    ) {
			extracount++;
		}
	}
	return((extracount*10)+count);
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

int is_valid(void)
{
	int i;
	for (i=0; i<81; i++) {
		if ( (field[i].value == 0) && (field[i].possible == 0) )
			return(0);
	}
	return(1);
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

void check_double_value_exact(void)
{
	int i;
	int mask;
	int a_brc[3] = {0, 0, 0};
	int brc=0;

	for (mask=ONE; mask<=ALL; mask++) {
		for (i=0; i<9; i++) {
			for (brc=0; brc<3; brc++) {
				bzero(a_brc, sizeof(int)*3);
				if (check_num_brc_exact(brc, a_brc, i, mask) == mtop(mask)) {
					fill_brc_double(brc, i, a_brc, mask);
					check_single();
				}
			}
		}

	}
}

void check_double_value_loose(void)
{
	int i;
	int mask;
	int a_brc[3] = {0, 0, 0};
	int brc=0;
	int ret=0;

	for (mask=ONE; mask<=ALL; mask++) {
		for (i=0; i<9; i++) {
			for (brc=0; brc<3; brc++) {
				bzero(a_brc, sizeof(int)*3);
				ret = check_num_brc_loose(brc, a_brc, i, mask);
				if ((ret < 10) && (ret == mtop(mask)) ) {
					fill_brc_double(brc, i, a_brc, mask);
					check_single();
				}
			}
		}
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

int readfield(const char *filename)
{
	FILE *fd;
	int i;
	char number;
	extern struct square field[81];

	if (!(fd = fopen(filename, "r"))) {
		printf("error opening file\n");
		return -1;
	}

	for (i=0; i<81; i++) {
		number = fgetc(fd);
		if ((number < 0x30) || (number > 0x3a)) {
			/* the character read is actually not a number
			   so skip it and continue. */
			i--;
			continue;
		}
		field[i].value = (number & ~0x30);
		/* Set the initial flag
		   This will be used in printing the field*/
		if (field[i].value > 0) {
			field[i].initial = 1;
		} else {
			field[i].initial = 0;
		}
		field[i].bftry = 0x0;
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
	check_filled();
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
				printfield(wfield, 1);
				check_single();
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
				printfield(wfield, 1);
				check_single();
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
				printfield(wfield, 1);
				check_single();
			}
		}
		check_single();
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

int stuck(int *count, int *previousleft)
{
	char yesno;
	int left;
	int i;
	if (bruteforced > 1) {
		autobruteforce();
		left = (9*9)*9;
		*count = 0;
		fill_all();
		check_filled();
		printfield(wfield, 1);
	} else {
		winprintf(wfield, "unsolvable?\n\r");
		wrefresh(wfield);
		printfield(wfield, 1);
		winprintf(wtext, "\n\rDo you wish to bruteforce\nthe sudoku? [a/y/N]");

		yesno = wgetch(wtext);
		if(yesno == 'Y' || yesno == 'y') {
			/* bruteforce */
			bruteforce();
			/* reset the values since they are no longer to be trusted. */
			left = (9*9)*9;
			*count = 0;
			fill_all();
			check_filled();
			printfield(wfield, 1);

		} else if(yesno == 'A' || yesno == 'a') {
			/* Automatically brute force the sudoku */
			autobruteforce();
			left = (9*9)*9;
			*count = 0;
			fill_all();
			check_filled();
			printfield(wfield, 1);
		} else {
			/* Print the remaining array, so we can save it */
			for (i=0; i<81; i++)
				winprintf(wfield, "%d", field[i].value);
			winprintf(wfield, "\n\r");
			wrefresh(wfield);
			endwin();
			exit(1);
		}
	}
	return left;
}

int final_check(void)
{
	int where;
	int brc;
	int i;
	int mask;

	winprintf(wtext, "\n\rFinal check.\n\r");
	for (where=0; where<9; where++) {
		for (brc=0; brc<3; brc++) {
			mask=0x0;

			/* print the field using highlights. */
			bf_printfield(brc, where);
			for (i=0; i<81; i++) {
				if (i_to_brc(brc, i) == where) {
					mask |= vtom(field[i].value);
				}
			}
			if (mask != ALL)
				return 0;
		}
	}
	return 1;
}

int invalid(void)
{
	winprintf(wtext, "\n\rSudoku is invalid.\n\r");
	if (bruteforced == 1) {
		if (prompt_yesno(1, "Do you wish to restore the backup?")) {
			winprintf(wtext, "\n\rRestoring backup.",
				  " You should try again.\n\r");
			if (restore_backup() == -1)
				winprintf(wtext, "Restore not possible\n\r");
			bruteforced = 0;
		}
	}
	if (bruteforced > 1) {
		if (restore_backup() == -1)
			winprintf(wtext, "\n\rRestore not possible");
	}
	return 0;
}

int solve(int *count, int *previousleft)
{
	int left = (9 * 9) * 9;

	if (!is_valid()) {
		invalid();
	}
	printfield(wfield, 1);
	check_filled();
	printfield(wfield, 1);

	check_only();
	printfield(wfield, 1);

	check_filled();
	printfield(wfield, 1);

	left = get_left();

	if (left != *previousleft) {
		*previousleft = left;
		*count = 0;
	} else {
		switch (*count) {
		case 0:
			check_single();
			break;
		case 1:
			check_double();
			break;
		case 2:
			check_double_value_exact();
			break;
		case 3:
			check_double_value_loose();
			break;
		case 4:
			if (left > 0) {
				left = stuck(count, previousleft);
			}
			break;
		}
	}
	*count += 1;
	return left;
}
