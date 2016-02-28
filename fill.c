#include "config.h"

#ifdef HAVE_NCURSES
#include <ncurses.h>
#endif /* HAVE_NCURSES */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "const.h"
#include "s_util.h"
#include "bf_util.h"

struct single row[9];
struct single col[9];
struct single block[9];

int store_list(void)
{
	char *filename= malloc(80);
	FILE *f;
	int i;
	winprintf(wtext, "\n\rEnter the filename to store the sudoku in (empty"
		  " to solve the puzzle): ");
	wscanw(wtext, "%80s", filename);

	if (strlen(filename) == 0)
		return 0;

	if (!(f = fopen(filename, "w"))) {
		winprintf(wtext, "Could not open file %s", filename);
		return -1;
	}
	for (i = 0; i < 81; i++) {
		fprintf(f, "%d", field[i].value);
	}
	fprintf(f, "\n");
	fclose(f);
	return 1;
}

void print_list(void)
{
	int i;
	for (i = 0; i < 81; i++)
		winprintf(wfield, "%d", field[i].value);
}

int main(int argc, char **argv)
{
	int i;
	int ret;
	int left = (9*9)*9;
	int count = 0;
	int strict = 0;
	extern struct square field[81];

	if (argc == 2) {
		if (!strcmp(argv[1], "--strict") || !strcmp(argv[1], "-s"))
			strict = 1;
	}

	bf_backups = malloc(sizeof(struct bf_backups));

	for (i = 0; i < 81; i++) {
		field_init(i);
	}
	if (initialize())
		return 1;

	fill_all();
	left = get_left();

	for (i = 0; i < 81; i++) {
		bf_printfield(ROW, -1, i);
		ret = bf_enter_value(i);
		if (ret > 0) {
			field[i].initial = 1;
		} else if (ret == -1) {
			i--;
			field_init(i);
			fill_all();
			i--;
			continue;
		} else if (ret == -2) {
			break;
		}
		if (!strict) {
			left = solve_run(&count);
			count = 0;
		}
	}

	if (!store_list()) {
		count = 0;
		fill_all();
		left = get_left();
		while (left > 0) {
			check_validity();
			solve_run(&count);
			check_validity();
			left = get_left();
			count++;
		}
	}
	print_list();
	press_any_key();
	endwin();
	return 0;
}
