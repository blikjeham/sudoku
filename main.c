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

int main(int argc, char **argv)
{
	FILE *fd;
	int left = (9*9)*9;
	int count=0;

	bf_backups = malloc(sizeof(struct bf_backups));

	if (!(fd = fopen("fields.sud", "r"))) {
		printf("error opening file\n");
		exit(1);
	}
	if(readfield(fd)) {
		printf("error reading field\n");
		exit(1);
	}

	if (initialize())
		exit(1);

	fill_all();
	check_filled();
	printfield(wfield, 1);
	bruteforced = 0;

	/* main loop */
	while (left > 0) {
		check_validity();
		left = solve_run(&count);
		count++;
	}
	if (final_check()) {
		winprintf(wtext, "\n\rSolved successfully\n\r");
	} else {
		winprintf(wtext, "\n\rThere are still some errors\n\r");
	}
	printfield(wfield, 1);
	wgetch(wtext);
	endwin();
	return(0);
}
