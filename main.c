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
			fill_brc(BLOCK, i_to_brc(BLOCK, i), field[i].value);
			fill_brc(ROW, i_to_brc(ROW, i), field[i].value);
			fill_brc(COL, i_to_brc(COL, i), field[i].value);
		} else {
			if (field[i].left == 1) {
				field[i].value = mtov(field[i].possible);
				field[i].left--;
			}
		}
	}
}

void check_single(void)
{
	int i,num;
	for (num=1; num<10; num++) {
		for (i=0; i<9; i++) {
			if (check_num(BLOCK, i, vtom(num)) == 1) {
				set_num(BLOCK, i, num);
			}
			check_filled();
			if (check_num(ROW, i, vtom(num)) == 1) {
				set_num(ROW, i, num);
			}
			check_filled();
			if (check_num(COL, i, vtom(num)) == 1) {
				set_num(COL, i, num);
			}
			check_filled();
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
	int left = 81;
	int previousleft = 81;

	if (!(fd = fopen("fields.sud", "r"))) {
		printf("error opening file\n");
		exit(1);
	}
	if(readfield(fd)) {
		printf("error reading field\n");
		exit(1);
	}
	fill_all();
	printfield();
	check_filled();
	printfield();
	/* main loop */
	while (left > 0) {
		printfield();
		check_single();
		left = get_left();
		printf("left: %d\n", left);
		if (left != previousleft) {
			previousleft = left;
		} else {
			printf("unsolvable?\n");
			exit(1);
		}
	}
	printfield();
	return(0);
}
