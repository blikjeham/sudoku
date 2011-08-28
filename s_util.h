/* s_util.h
 *
 * sudoku utilities (i.e. helper functions)
 */

#ifndef S_UTIL_H
#define S_UTIL_H 1

#include <stdio.h>

/* print the field so far */
void printfield(int );

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

int check_num_tripple(int, int *, int, int);

int check_num(int, int, int);

void set_num(int, int, int);

/* Remove the possibilities for a value from a block, row or  column */
void fill_brc(int, int , int );

/* value to mask
   mask to value */
int vtom(int );
int mtov(int );

/* convert a possibilities field to a single value */
int get_value(int );

#endif /* S_UTIL_H */
