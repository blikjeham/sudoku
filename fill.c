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
	extern struct square field[81];

	bf_backups = malloc(sizeof(struct bf_backups));

	for (i = 0; i < 81; i++) {
		field_init(i);
	}
	if (initialize())
		return 1;

	fill_all();
	left = get_left();

	for (i = 0; i < 81; i++) {
		bf_printfield(ROW, i/9, i);
		ret = bf_enter_value(i);
		if (ret > 0) {
			field[i].initial = 1;
		} else if (ret < 0) {
			i--;
			field_init(i);
			fill_all();
			i--;
			continue;
		}
		left = solve_run(&count);
		count = 0;
	}

	count = 0;
	fill_all();
	left = get_left();
	while (left > 0) {
		check_validity();
		left = solve_run(&count);
		count++;
	}

	print_list();
	wgetch(wtext);
	endwin();
	return 0;
}
