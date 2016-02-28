#include "config.h"

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "const.h"
#include "bf_util.h"
#include "s_util.h"

static int brc_to_i(int brc, int which, int where)
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

static int is_bf_able(int where)
{
	if (field[where].initial > 0) {
	    winprintf(wtext, "\n\rInitial value: %d\n\r", field[where].initial);
	    return(0);
	}
	if (field[where].value > 0) {
	    winprintf(wtext, "\n\rValue: %d\n\r", field[where].value);
	    return(0);
	}
	if (field[where].bftry == field[where].possible) {
	    winprintf(wtext, "\n\rAlready tried those possibilities\n\r");
	    return(0);
	}
	return(1);
}

static int is_abf_able(int where)
{
	if ((field[where].initial > 0) || (field[where].value > 0) || field[where].bftry == field[where].possible) {
		return(0);
	} else {
		return(1);
	}
}

static void bf_set_value(int i, int value)
{
	field[i].value = value;
	field[i].possible = 0x0;
	field[i].left = 0;
	field[i].bftry |= vtom(value);
	bf_backups->current[i].bftry |= vtom(value);
}

int bf_enter_value(int i)
{
	int num;
	int value=-1;

	winprintf(wtext, "\n\rPossible values are: ");
	for (num=1; num<=9; num++) {
		if (field[i].possible & vtom(num))
			winprintf(wtext, "%d ", num);
	}
	winprintf(wtext, "\n\rAlready tried: ");
	for (num=1; num <=9; num++) {
		if (field[i].bftry & vtom(num))
			winprintf(wtext, "%d ", num);
	}
	while (value<0 || value>9) {
		winprintf(wtext, "\n\rEnter a value: ");
		value = wgetch(wtext);
		if (value == KEY_BACKSPACE || value == '-') {
			return -1;
		} else if (value == 'q') {
			return -2;
		}
		value = value % 0x30;
	}
	if (field[i].possible & vtom(value)) {
		bf_set_value(i, value);
	} else if (value == 0) {
		field[i].initial = 0;
		return 0;
	} else {
		return bf_enter_value(i);
	}
	return 1;
}

static void bf_getfield(int brc, int where)
{
	int which=-1;
	int i=-1;

	wclear(wfield);
	wrefresh(wfield);

	bf_printfield(brc, where, -1);
	while (which<0 || which>8) {
		winprintf(wtext, "\n\rPlease select a field:");
		which = wgetch(wtext) % 0x30;
	}
	i = brc_to_i(brc, where, which);
	if (is_bf_able(i)) {
		bf_enter_value(i);
	} else {
		sleep(3);
		bruteforce();
	}
	
}

static void bf_block(void)
{
	int block=-1;
	while (block<0 || block>8) {
		winprintf(wtext, "\n\rPlease select a block:");
		block = wgetch(wtext) % 0x30;
	}
	winprintf(wtext, "\n\rselected block %d\n\r", block);
	bf_getfield(BLOCK, block);
}

static void bf_row(void)
{
	int row=-1;
	while (row<0 || row>8) {
		winprintf(wtext, "\n\rPlease select a row:");
		row = wgetch(wtext) % 0x30;
	}
	winprintf(wtext, "\n\rselected row %d\n\r", row);
	bf_getfield(ROW, row);
}

static void bf_col(void)
{
	int col=-1;
	while (col<0 || col>8) {
		winprintf(wtext, "\n\rPlease select a column:");
		col = wgetch(wtext) % 0x30;
	}
	winprintf(wtext, "\n\rselected column %d\n\r", col);
	bf_getfield(COL, col);
}

void bf_printfield(int brc, int where, int j)
{
	int i;
	wmove(wfield, 0,0);

	winprintf(wfield, "    0   1   2     3   4   5     6   7   8\n\r");
	winprintf(wfield, " +-------------+-------------+-------------+\n\r0| ");
	for (i=0; i<81; i++) {
		/* highlight the correct BRC */
		if (i_to_brc(brc, i) == where) {
			wcolor_set(wfield, 2, NULL);
		} else if (i == j) {
			/* highlight current selection */
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

static int make_backup(void)
{
	struct bf_backups *new;
	int i;

	new = (struct bf_backups *)malloc(sizeof(struct bf_backups));
	memcpy(new, bf_backups, sizeof(struct bf_backups));
	new->depth++;
	new->previous = (struct bf_backups *)bf_backups;
	
	for (i=0; i<81; i++) {
		new->current[i] = field[i];
	}

	bf_backups = new;
	winprintf(wtext, "\n\rM-Backup depth: %d", bf_backups->depth);
	return(0);
}

int restore_backup(void)
{
	struct bf_backups *old;
	int i;
	if (bf_backups->previous == NULL)
		return(-1);

	for (i=0; i<81; i++) {
		field[i] = bf_backups->current[i];
	}
	old = bf_backups;
	bf_backups = bf_backups->previous;
	free(old);
	winprintf(wtext, "\n\rR-Backup depth: %d", bf_backups->depth);
	return(0);
}

void bruteforce(void)
{
	char brc=0;
	wclear(wtext);
	wrefresh(wtext);
	winprintf(wtext, "\n\rBacking up the field for undo.");
	make_backup();
	bruteforced = 1;


	winprintf(wtext, "\n\rSelect a block, row, or\ncolumn [b/r/c]?");
	brc = wgetch(wtext);
	switch(brc) {
	case 'b':
	case 'B':
		bf_block();
		break;
	case 'r':
	case 'R':
		bf_row();
		break;
	case 'c':
	case 'C':
		bf_col();
		break;
	default:
		winprintf(wtext, "invalid choice.\n\r");
		bruteforce();
		break;
	}
}

static int find_first_abfable(void)
{
	int i = 0;
	for (i=0; i<81; i++) {
		if(is_abf_able(i)) {
			return(i);
		}
	}
	return(-1);
}

static int find_best_abfable(void)
{
	int i = 0;
	int bestfield = -1;
	int mincount = 0;

	for (i=0; i<81; i++) {
		if ( (is_abf_able(i))
		     && (field[i].left > mincount)
		     ) {
			bestfield = i;
			mincount = field[i].left;
		}
	}
	return(bestfield);
}

void autobruteforce(void)
{
	int i = 0;
	int num = 0;

	/* find the first field that is bruteforcable */
	i = find_best_abfable();
	if (i == -1) {
		winprintf(wtext,
			  "\n\rNo more brutefore possibilities."
			  "\n\rRestoring backup");
		restore_backup();
		return;
	}

	make_backup();

	if (!bruteforced)
		bruteforced = 2;
	else
		bruteforced++;

	winprintf(wtext, "\n\rtries %d", bruteforced);
	for (num=1; num<10; num++) {

		if( (field[i].possible & vtom(num)) &&
		    !(field[i].bftry & vtom(num)) ) {

			winprintf(wtext,
				  "\n\rbruteforcing %d(%dx%d) value %d", i,
				  i_to_brc(COL, i), i_to_brc(ROW, i), num);

			bf_set_value(i, num);
		}
	}
}

void check_validity(void)
{
	char yesno;
	if (!is_valid()) {
		winprintf(wtext, "\n\rSudoku is invalid.\n\r");
		if (bruteforced == 1) {
			winprintf(wtext,
				  "Do you wish to restore the backup? [Y/n]");
			yesno = wgetch(wtext);
			if (yesno != 'n' && yesno != 'N') {
				winprintf(wtext,
					  "\n\rRestoring backup. You should try"
					  "again.\n\r");
				if (restore_backup() == -1)
					winprintf(wtext,
						  "Restore not possible\n\r");
				bruteforced = 0;
			}

		}
		if (bruteforced > 1) {
			if (restore_backup() == -1)
				winprintf(wtext, "\n\rRestore not possible");
		}
	}
}

int start_bruteforce(void)
{
	char yesno;
	int count;
	int i;
	int left;
	if (bruteforced > 1) {
		autobruteforce();
		left = (9*9)*9;
		count = 0;
		fill_all();
		check_filled();
		printfield(wfield, 1);
	} else {
		winprintf(wtext, "\n\runsolvable?\n\r");
		wrefresh(wfield);
		printfield(wfield, 1);
		winprintf(wtext,
			  "\n\rDo you wish to bruteforce\nthe sudoku? [a/y/N]");

		yesno = wgetch(wtext);
		if(yesno == 'Y' || yesno == 'y') {
			/* bruteforce */
			bruteforce();
			/* reset the values since they are no longer to be */
			/* trusted. */
			left = (9*9)*9;
			count = 0;
			fill_all();
			check_filled();
			printfield(wfield, 1);

		} else if(yesno == 'A' || yesno == 'a') {
			/* Automatically brute force the sudoku */
			autobruteforce();
			left = (9*9)*9;
			count = 0;
			fill_all();
			check_filled();
			printfield(wfield, 1);

		} else {
			/* Print the remaining array, so we can save it */
			for (i=0; i<81; i++)
				winprintf(wfield, "%d", field[i].value);
			winprintf(wfield, "\n\r");
			wrefresh(wfield);
			wgetch(wtext);
			endwin();
			exit(1);
		}
	}
	return count;
}
