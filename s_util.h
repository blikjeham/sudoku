/* s_util.h
 *
 * sudoku utilities (i.e. helper functions)
 */

#ifndef S_UTIL_H
#define S_UTIL_H 1

#include <stdio.h>
#include <ncurses.h>

/* printf to a window */
void winprintf(WINDOW *, char *, ...);

/* check for impossibilities */
void check_filled(void);

/* check for only possibility */
void check_only(void);

/* check for single value in BRC */
void check_single(void);

/* check for double possibilities in BRC */
void check_double(void);

/* check for double value possibilities in BRC */
void check_double_value(void);

/* get the number of values left */
int get_left(void);

/* print the field so far */
void printfield(WINDOW *, int );

/* read the values from a file */
int readfield(FILE *);

/* fill all empty fields with all possibilities
   We will remove these possibilities one by one
   later on. */
void fill_all(void);

/* these functions will convert a number
   between 0 and 81 to a block, row, or column ranging
   from 0 to 9 */
int i_to_brc(int, int );

int check_num_brc(int, int *, int, int);

int check_num(int, int, int);

void set_num(int, int, int);

/* Remove the possibilities for a value from a block, row or  column */
void fill_brc(int, int , int );

void fill_brc_ex(int , int , int *, int );

void fill_brc_double(int , int , int *, int );

/* convert mask to number of possibilities */
int mtop(int );

/* value to mask
   mask to value */
int vtom(int );
int mtov(int );

/* convert a possibilities field to a single value */
int get_value(int );

#endif /* S_UTIL_H */
