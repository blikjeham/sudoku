#include <stdio.h>
#include <stdlib.h>

#include "const.h"
#include "s_util.h"

struct single row[9];
struct single col[9];
struct single block[9];

void check_filled(void)
{
	int i;
	
	for (i=0; i<81; i++) {
		if (field[i].value > 0) {
			fill_block(i_to_block(i), field[i].value);
			fill_row(i_to_row(i), field[i].value);
			fill_col(i_to_col(i), field[i].value);
		} else {
			if (field[i].left == 1) {
				field[i].value = mtov(field[i].possible);
				field[i].left--;
			}
		}
	}
}

int get_left(void)
{
	int i;
	int left=0;
	for (i=0; i<81; i++) {
		left += field[i].left;
	}
	return(left);
}

int main(int argc, char **argv)
{
	FILE *fd;
	int left;

	if (!(fd = fopen("fields.sud", "r"))) {
		printf("error opening file\n");
		exit(1);
	}
	if(readfield(*fd)) {
		printf("error reading field\n");
		exit(1);
	}
	
	fill_all();

	/* main loop */
	while (left > 0) {
		check_filled();
		left = get_left();
	}
	
	return(0);
}
