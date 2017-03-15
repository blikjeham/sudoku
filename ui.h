#ifndef UI_H
#define UI_H

#include "config.h"

#ifdef HAVE_NCURSES
#include <ncurses.h>
#else
typedef void WINDOW;
#endif

extern WINDOW *wfield;
extern WINDOW *wtext;

/* printf to a window if we have ncurses
 * else we will print it to stdout.
 */
void winprintf(WINDOW *, char *, ...);

/* print the field so far
 * If the second argument is true, print the
 * possibilities alongside the actual values.
 * That should make debugging easier.
 */
void printfield(WINDOW *, int );

int prompt_yesno(int , char *, ...);

#endif /* UI_H */
