#include "config.h"

#ifdef HAVE_NCURSES
#include <ncurses.h>
#endif /* HAVE_NCURSES */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "const.h"
#include "ui.h"
#include "s_util.h"
#include "bf_util.h"

struct single row[9];
struct single col[9];
struct single block[9];

int init(const char *filename)
{
	if (readfield(filename) < 0)
		return -1;

#ifdef HAVE_NCURSES
	/* Initialize ncurses screen */
	if (!initscr()) {
		printf("Error initializing ncurses\n");
		return -1;
	}
	/* Initialize the colors */
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);

	/* Set the windows */
	wfield = newwin(20,46,3,3);
	wtext = newwin(18,25,3,50);
	scrollok(wtext, TRUE);

#else /* HAVE_NCURSES */

	wfield = NULL;
	wtext = NULL;
#endif /* HAVE_NCURSES */

	fill_all();
	check_filled();
	printfield(wfield, 1);
	bruteforced = 0;

	return 0;
}

int main(int argc, char **argv)
{
	int left = 9 * 9 * 9;
	int count = 0;
	int previousleft = 9 * 9 * 9;

	bf_backups = malloc(sizeof(struct bf_backups));

	if (init("fields.sud") < 0) {
		return -1;
	}

	/* main loop */
	while (left > 0) {
		left = solve(&count, &previousleft);
	}
	if (final_check()) {
		winprintf(wtext, "\n\rSolved successfully\n\r");
	} else {
		winprintf(wtext, "\n\rThere are still some errors\n\r");
	}
	printfield(wfield, 1);

	endwin();
	return 0;
}
