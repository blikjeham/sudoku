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
	int brc;
	
	for (i=0; i<81; i++) {
		if (field[i].value > 0) {
			for (brc=0; brc<3; brc++) {
				fill_brc(brc, i_to_brc(brc, i), field[i].value);
			}
		} else {
			if (field[i].left == 1) {
				field[i].value = mtov(field[i].possible);
				printf("row: %d, value: %d\n", i_to_brc(ROW, i), field[i].value);
				field[i].left--;
			}
		}
	}
}

void check_single(void)
{
	int i,num;
	int brc;
	for (num=1; num<10; num++) {
		for (i=0; i<9; i++) {
			for (brc=0; brc<3; brc++) {
				if (check_num(brc, i, vtom(num)) == 1) {
					set_num(brc, i, num);
					check_filled();
				}
			}
		}
	}
}

void check_double(void)
{
	int num;
	int i,j;
	int a_brc[3] = {0, 0, 0};
	for (num=1; num<10; num++) {
		for (i=0; i<9; i++) {
			bzero(a_brc, sizeof(int)*3);
			if (check_num_tripple(BLOCK, a_brc, i, vtom(num)) == 2) {
				printf("found in blk(%d) %d, a_brc[ROW] %d, a_brc[COL] %d, value %d\n", BLOCK, i, a_brc[ROW], a_brc[COL], num);
				fill_brc_ex(BLOCK, i, a_brc, vtom(num));
			}
			if (check_num_tripple(ROW, a_brc, i, vtom(num)) == 2) {
				printf("found in row(%d) %d, a_brc[BLK] %d, a_brc[COL] %d, value %d\n", ROW, i, a_brc[BLOCK], a_brc[COL], num);
				fill_brc_ex(ROW, i, a_brc, vtom(num));
			}
			if (check_num_tripple(COL, a_brc, i, vtom(num)) == 2) {
				printf("found in col(%d) %d, a_brc[BLK] %d, a_brc[ROW] %d, value %d\n", COL, i, a_brc[BLOCK], a_brc[ROW], num);
				printf("found in col %d, value %d\n", i, num);
				fill_brc_ex(COL, i, a_brc, vtom(num));
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
	int left = 81;
	int previousleft = 81;
	int count=0;

	if (!(fd = fopen("fields.sud", "r"))) {
		printf("error opening file\n");
		exit(1);
	}
	if(readfield(fd)) {
		printf("error reading field\n");
		exit(1);
	}
	fill_all();
	printfield(0);
	check_filled();
	printfield(1);
	/* main loop */
	while (left > 0) {
		printfield(0);
		check_single();
		printfield(1);
		check_double();
		printfield(1);
		left = get_left();
		printf("left: %d\n", left);
		if (left != previousleft) {
			previousleft = left;
			count++;
		} else if (count == 3 && left > 0){
			printf("unsolvable?\n");
			printfield(0);
			printfield(1);
			exit(1);
		}
	}
	printfield(1);
	printfield(0);
	return(0);
}
