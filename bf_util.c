#include "config.h"

#include <ncurses.h>

#include "const.h"
#include "bf_util.h"
#include "s_util.h"

int is_bf_able(int where)
{
	if ((field[where].initial > 0) || (field[where].value > 0)) {
		winprintf(wtext, "\n\rNot bruteforcable (%d)\n\r", where);
		return(0);
	}
	else {
		return(1);
	}
}

void bf_set_value(int where)
{
	int i;
	int value;
       
	winprintf(wtext, "\n\rPossible values are: ");
	for (i=1; i<=9; i++) {
		if (field[where].possible & vtom(i))
			winprintf(wtext, "%d ", i);
	}
	winprintf(wtext, "\n\rEnter a value: ");
	value = wgetch(wtext) % 0x30;
	if (field[where].possible & vtom(value)) {
		field[where].value = value;
		field[where].possible = 0x0;
		field[where].left = 0;
	}
}

void bf_getfield(int brc, int brc_where)
{
	int which, where;

	wclear(wfield);
	wrefresh(wfield);

	bf_printfield(brc, brc_where);
	winprintf(wtext, "\n\rPlease select a field:");
	which = wgetch(wtext) % 0x30;
	where = brc_to_i(brc, brc_where, which);
	if (is_bf_able(where)) {
		bf_set_value(where);
	}
	
}

void bf_block(void)
{
	int block;
	winprintf(wtext, "\n\rPlease select a block:");
	block = wgetch(wtext) % 0x30;
	winprintf(wtext, "\n\rselected block %d\n\r", block);
	bf_getfield(BLOCK, block);
}

void bf_row(void)
{
	int row;
	winprintf(wtext, "\n\rPlease select a row:");
	row = wgetch(wtext) % 0x30;
	winprintf(wtext, "\n\rselected row %d\n\r", row);
	bf_getfield(ROW, row);
}

void bf_col(void)
{
	int col;
	winprintf(wtext, "\n\rPlease select a column:");
	col = wgetch(wtext) % 0x30;
	winprintf(wtext, "\n\rselected column %d\n\r", col);
	bf_getfield(COL, col);
}

int brc_to_i(int brc, int which, int where)
{
	if (brc == BLOCK) {
		return( ((((which/3)*3)*9) + ((which%3)*3)) + (((where/3)*9)+where%3) );
	}
	if (brc == ROW) {
		return((which*9)+where);
	}
	if (brc == COL) {
		return((where*9)+which);
	}
	return(-1);
}

void bf_printfield(int brc, int where)
{
	int i;
	wmove(wfield, 0,0);
	wclear(wfield);
	wrefresh(wfield);

	winprintf(wfield, "    0   1   2     3   4   5     6   7   8\n\r");
	winprintf(wfield, " +-------------+-------------+-------------+\n\r0| ");
	for (i=0; i<81; i++) {
		/* highlight the correct BRC */
		if (i_to_brc(brc, i) == where) {
			wcolor_set(wfield, 2, NULL);
		}
		if (field[i].value == 0) {
			winprintf(wfield, "%03d ", field[i].possible);
		} else {
			/* Print initial values different than later values */
			if (field[i].initial) {
				winprintf(wfield, " %d  ", field[i].value);
			} else {
				winprintf(wfield, ".%d. ", field[i].value);
			}
		}
		/* Reset the color */
		wcolor_set(wfield, 1, NULL);
		if ((i % 81) != 80) {
			if ((i % 3) == 2)
				winprintf(wfield, "| ");
			if ((i % 27) == 26) {
				winprintf(wfield, "\n\r +-------------+-------------+-------------+");
			}
			if ((i % 9) == 8)
				winprintf(wfield, "\n\r%d| ", (i/9)+1);
		} else {
			winprintf(wfield, "|\n\r");
		}
	       
	}
	winprintf(wfield, " +-------------+-------------+-------------+\n\r\n\r");
	
	for (i=1; i<10; i++) {
		winprintf(wfield, "%d=%d; ", i, vtom(i));
		if (i % 5 == 0)
			winprintf(wfield, "\n\r");
	}
	winprintf(wfield, "\n\r");

	winprintf(wfield, "left: %d\n\r", get_left());
}
