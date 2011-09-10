#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "s_util.h"

struct single row[9];
struct single col[9];
struct single block[9];

int main(int argc, char **argv)
{
	int i=1;
	int num;
	int mask;
	if (argc < 3 ) {
		printf("Not enoug parameters. Please supply a number\n");
		exit(1);
	}
	mask = atoi(argv[1]);
	num = atoi(argv[2]);
	if (num > 0) {
		printf("%d consists of: ", num);
		for (i=1; i<10; i++) {
			if (num & vtom(i)) {
				printf("%d ", i);
			}
		}
		printf("\n");
		printf("%d consists of: ", mask);
		for (i=1; i<10; i++) {
			if (mask & vtom(i)) {
				printf("%d ", i);
			}
		}
		printf("\n");
		if ((num & mask) == mask) {
			printf("%d & %d match\n", num, mask);
		}
	}
	return 0;
}
