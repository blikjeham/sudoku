#include "config.h"
#include "const.h"
#include "ui.h"
#include "s_util.h"

WINDOW *wfield;
WINDOW *wtext;

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
			if (possible) {
				/* Print initial values different than later values */
				if (field[i].initial) {
					winprintf(wfield, " %d  ", field[i].value);
				} else {
					winprintf(wfield, ".%d. ", field[i].value);
				}
			} else {
				winprintf(wfield, "%d ", field[i].value);
			}
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
	winprintf(wfield, "left: %d\n\r", get_left());
}

int prompt_yesno(int def, char *fmt, ...)
{
	char yesno;
	va_list va;

	va_start(va, fmt);
	winprintf(wtext, fmt, va);
	va_end(va);

	if (def) {
		winprintf(wtext, " [Y/n]");
	} else {
		winprintf(wtext, " [y/N]");
	}

	yesno = wgetch(wtext);

	if (yesno == 'N' || yesno == 'n')
		return 0;

	if (yesno == 'Y' || yesno == 'y')
		return 1;

	return def;
}
