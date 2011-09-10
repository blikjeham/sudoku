#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "const.h"
#include "s_util.h"

struct single row[9];
struct single col[9];
struct single block[9];


int main(int argc, char **argv)
{
	FILE *fd;
	int left = 81;
	int previousleft = 81;
	int count=0;
	int i;

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
		check_filled();
		printfield(0);
		check_single();
		check_filled();
		printfield(1);

		left = get_left();
		printf("left: %d\n", left);
		if (left != previousleft) {
			previousleft = left;
			count=0;
		} else {
			if (count == 0) 
				check_only();
			if (count == 1)
				check_double();
			if (count == 2)
				check_double_value();

			if (count == 3 && left > 0){
				printf("unsolvable?\n");
				printfield(0);
				printfield(1);

				/* Print the remaining array, so we can save it */
				for (i=0; i<81; i++)
					printf("%d", field[i].value);
				printf("\n");
				exit(1);
			}
			count++;
		}
	}

	printfield(0);
	return(0);
}
