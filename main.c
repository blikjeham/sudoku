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
	int left = 81;
	int previousleft = 81;
	int count=0;
	int i;
	char yesno;
	
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
	/* Initialize the colors */
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);

	/* Set the windows */
	wfield = newwin(20,46,3,3);
	wtext = newwin(18,46,3,50);
	scrollok(wtext, TRUE);

#else /* HAVE_NCURSES */

	wfield = NULL;
	wtext = NULL;
#endif /* HAVE_NCURSES */
	
	fill_all();
	check_filled();
	printfield(wfield, 1);
	bruteforced = 0;

	/* main loop */
	while (left > 0) {
		if (!is_valid()) {
			winprintf(wtext, "\n\rSudoku is invalid.\n\r");
			if (bruteforced == 1) {
				winprintf(wtext, "Do you wish to restore the backup? [Y/n]");
				yesno = wgetch(wtext);
				if (yesno != 'n' && yesno != 'N') {
					winprintf(wtext, "\n\rRestoring backup. You should try again.\n\r");
					memcpy(field, bf_backup, sizeof(struct square[81]));
					bruteforced = 0;
				}
			}
		}
		printfield(wfield, 1);
		check_filled();
		printfield(wfield, 1);

		check_single();
		printfield(wfield, 1);

		check_filled();
		printfield(wfield, 1);

		left = get_left();
		if (left != previousleft) {
			previousleft = left;
			count=0;
		} else {
			if (count == 0) {
				check_only();
			}
			if (count == 1) {
				check_double();
			}
			if (count == 2) {
				check_double_value_exact();
			}
			if (count == 3) {
				check_double_value_loose();
			}

			if (count == 4 && left > 0){
				winprintf(wfield, "unsolvable?\n\r");
				wrefresh(wfield);
				printfield(wfield, 1);
				winprintf(wtext, "\n\rDo you wish to bruteforce the sudoku? [y/N]");

				yesno = wgetch(wtext);
				if(yesno == 'Y' || yesno == 'y') {
					/* bruteforce */
					bruteforce();
					/* reset the values since they are no longer to be trusted. */
					left = 81;
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
					endwin();
					exit(1);
				}

			}
			count++;
		}
	}
	printfield(wfield, 1);

	endwin();
	return(0);
}
