#ifndef BF_UTIL_H
#define BF_UTIL_H 1

/* We can't bruteforce a value that was already set.
 * Check if it is bruteforceable.
 */
int is_bf_able(int );

/* ask for user to set the value */
void bf_set_value(int );

/* Bruteforce the BRC. */
void bf_block(void);
void bf_row(void);
void bf_col(void);

/* convert a field within a BRC to the global field */
int brc_to_i(int , int , int );

/* Print the field with the BRC highlighted that we want
 * to bruteforce
 */
void bf_printfield(int , int );

#endif /* BF_UTIL_H */
