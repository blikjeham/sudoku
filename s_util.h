/* s_util.h
 *
 * sudoku utilities (i.e. helper functions)
 */

#ifndef S_UTIL_H
#define S_UTIL_H 1

/* read the values from a file */
int readfield(FILE *);

/* fill all empty fields with all possibilities
   We will remove these possibilities one by one
   later on. */
void fill_all(void);

/* these functions will convert a number
   between 0 and 81 to a block, row, or column ranging
   from 0 to 9 */
int i_to_block(int );
int i_to_row(int );
int i_to_col(int );

/* value to mask
   mask to value */
int vtom(int );
int mtov(int );

/* */
void fill_block(int , int );

/* */
void fill_row(int , int );

/* */
void fill_col(int , int );

/* convert a possibilities field to a single value */
int get_value(int );

#endif /* S_UTIL_H */
