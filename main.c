#include "config.h"

#ifdef HAVE_NCURSES
#include <ncurses.h>
#endif /* HAVE_NCURSES */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "const.h"
#include "s_util.h"

struct single row[9];
struct single col[9];
struct single block[9];


int main(int argc, char **argv)
{
	FILE *fd;
	int left = 81;
	int previousleft = 81;
	int count=0;
	int i;

	if (!(fd = fopen("fields.sud", "r"))) {
		printf("error opening file\n");
		exit(1);
	}
	if(readfield(fd)) {
		printf("error reading field\n");
		exit(1);
	}

#ifdef HAVE_NCURSES
	/* Initialize ncurses screen */
	if (!initscr()) {
		printf("Error initializing ncurses\n");
		exit(1);
	}

	wfield = newwin(46,46,3,3);
	wtext = newwin(10,50,3,50);
#else /* HAVE_NCURSES */
	wfield = NULL;
	wtext = NULL;
#endif /* HAVE_NCURSES */
	
	fill_all();
	//	printfield(wfield, 0);
	check_filled();
	printfield(wfield, 1);

	/* main loop */
	while (left > 0) {
		printfield(wfield, 1);
		winprintf(wtext, "check_filled\n\r");
		check_filled();
		printfield(wfield, 1);
		winprintf(wtext, "check_single\n\r");
		check_single();
		printfield(wfield, 1);
		winprintf(wtext, "check_filled\n\r");
		check_filled();
		printfield(wfield, 1);

		wrefresh(wfield);
		if (left != previousleft) {
			previousleft = left;
			count=0;
		} else {
			if (count == 0) {
				winprintf(wtext, "Checking only\n\r");
				check_only();
			}
			if (count == 1) {
				winprintf(wtext, "Checking double\n\r");
				check_double();
			}
			if (count == 2) {
				winprintf(wtext, "Checking double value (exact)\n\r");
				check_double_value_exact();
			}
			if (count == 3) {
				winprintf(wtext, "Checking double value (loose)\n\r");
				check_double_value();
			}

			if (count == 4 && left > 0){
				winprintf(wfield, "unsolvable?\n\r");
				wrefresh(wfield);
				printfield(wfield, 1);

				/* Print the remaining array, so we can save it */
				for (i=0; i<81; i++)
					winprintf(wfield, "%d", field[i].value);
				winprintf(wfield, "\n\r");
				wrefresh(wfield);
				endwin();
				exit(1);
			}
			count++;
		}
	}

	//	printfield(wfield, 0);

	endwin();
	return(0);
}
