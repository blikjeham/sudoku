/* s_util.h
 *
 * sudoku utilities (i.e. helper functions)
 */

#ifndef S_UTIL_H
#define S_UTIL_H 1

#include <stdio.h>
#include <ncurses.h>

/* printf to a window if we have ncurses
 * else we will print it to stdout.
 */
void winprintf(WINDOW *, char *, ...);

/* ask the user to press any key to continue */
void press_any_key(void);

/* check if the sudoku is still valid.
 * It is invalid if there is a field with
 * no value and no possibilities left.
 */
int is_valid(void);

/* check the field for values that are filled
 * Remove the possibilities that are no longer
 * possible.
 */
void check_filled(void);

/* check if a value is the only
 * possibility in the field
 */
void check_only(void);

/* check for single value within a BRC */
void check_single(void);

/* check for double possibilities in BRC
 * This applies to one value possible within
 * a block on the same row. Delete it then from
 * all the other fields within the block, and on
 * the row. This goes for all the BRCs.
 */
void check_double(void);

/* check for double value possibilities in BRC
 * This means n values are possible in only n
 * fields of the BRC. All it does is look if the
 * mask occurs exactly the same number of times
 * as the number of values composing this mask.
 * The exact function checks if the possibilities
 * is the mask exactly. The loose
 * function allows for other possibilities in the
 * same field.
 * Hence: loose matching should be used with care.
 */
void check_double_value_exact(void);
void check_double_value_loose(void);

/* get the number of possible values left */
int get_left(void);

/* print the field so far
 * If the second argument is true, print the
 * possibilities alongside the actual values.
 * That should make debugging easier.
 */
void printfield(WINDOW *, int );

/* read the values from a file
 * Returns 0 on success
 */
int readfield(FILE *);

/* fill all empty fields with all possibilities
 * We will remove these possibilities one by one
 * later on.
 * check_filled should be invoked as soon as
 * possible after fill_all.
 */
void fill_all(void);

/* these functions will convert a number
 * between 0 and 81 to a block, row, or column ranging
 * from 0 to 9
 */
int i_to_brc(int, int );

void set_num(int, int, int);

/* Remove the possibilities for a value from a BRC */
void fill_brc(int, int , int );

void fill_brc_ex(int , int , int *, int );

void fill_brc_double(int , int , int *, int );

/* value to mask
   mask to value */
int vtom(int );
int mtov(int );

/* convert a possibilities field to a single value */
int get_value(int );

/* Final validity check, just to be sure */
int final_check(void);

/* One solve run at a time. Supply pointer to count */
int solve_run(int *);

/* Initialize the field and screen */
void field_init(int );
int initialize(void);

#endif /* S_UTIL_H */
