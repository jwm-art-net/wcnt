#include <stdio.h>

#define MAXVAL 5000

struct notetype
{
	short qval;
	short smallest;
	bool use3;
	bool use5;
	bool use7;
} nt = {12, 16, 1, 0, 0};

void select_quarter_value();
void display_positions();
bool yaynay();

int main()
{
	short option;
	while(1) {
		printf("\nNote Length and Position Helper Program");
		printf("\n---------------------------------------");
		printf("\n\nQuarter value %d ", nt.qval);
		printf("smallest 1/%d ", nt.smallest);
		if (nt.use3) printf("+ triplets ");
		if (nt.use5) printf("+ quintuplets ");
		if (nt.use7) printf("+ septuplets");
		printf("\n\n\nPlease choose :-)\n");
		printf("\n\t1) Select a quarter note value");
		printf("\n\t2) Display positions across bar(s)");
		printf("\n\t3) Quit");
		printf("\n\nChoice :");
		scanf("%d", &option);
		if (option == 1) select_quarter_value();
		else if (option == 2) display_positions();
		else if (option == 3) return(0);
		else printf("\nBad Move, wrong attitude!\n");
	}
}

bool yaynay()
{
	char yn = 0;
	bool done = 0;
	while (1) {
		scanf("%c", &yn);
		switch (yn) {
			case 'y':
			case 'Y':
			return 1;
			case 'n':
			case 'N':
			return 0;
		}
	}
}

void select_quarter_value()
{
	printf("\nFancy note length Selector");
	printf("\n--------------------------");
	printf("\nDo you want to use triplets (y/n) [ENTER] ");
	nt.use3 = yaynay();
	printf("\nDo you want to use quintuplets (y/n) [ENTER] ");
	nt.use5 = yaynay();
	printf("\nDo you want to use septuplets (y/n) [ENTER] ");
	nt.use7 = yaynay();
	printf("\nSmallest note length selector");
	printf("\n-----------------------------\n");
	printf("\n\t1) 1/16");
	printf("\n\t2) 1/32");
	printf("\n\t3) 1/64");
	printf("\n\t4) 1/128");
	printf("\n\t5) 1/256");

	printf("\n\n\tChoice :");
	int opt = 0;
	while (opt < 1 || opt > 5) {
		scanf("%d", &opt);
		if (opt < 1 && opt > 5)
			printf("\noi! between 1 and 5.\n");
	}
	nt.smallest = 16;
	for (int i = 1; i < opt; i++)
		nt.smallest *= 2;
	float d2 = 0, d3 = 0, d5 = 0, d7 = 0;
	int i2 = 0, i3 = 0, i5 = 0, i7 = 0;
	for (int i = 1; i < MAXVAL; i++) {
		i2 = (int)(d2 = (double) i / 2);
		if (nt.use3) i3 = (int)(d3 = (double) i / 3);
		if (nt.use5) i5 = (int)(d5 = (double) i / 5);
		if (nt.use7) i7 = (int)(d7 = (double) i / 7);
		if (i2 == d2 && i3 == d3 && i5 == d5 && i7 == d7) {
			nt.qval = i * (nt.smallest / 8);
			return;
		}
	}
	return;
}

void display_positions()
{
	printf("\nLength and Position values");
	printf("\n--------------------------\n");
	int i;
	for (i = 0; i < 16;i ++) printf("\nbar %d\tpos %d", i, i * nt.qval * 4);
	short z = 1;
	i = nt.qval * 4;
	while (z <= nt.smallest) {
		printf("\n1/%d\t%d", z, i);
		if (nt.use3) printf("\ttriplet %d", i / 3);
		if (nt.use5) printf("\tquintuplet %d", i / 5);
		if (nt.use7) printf("\tseptuplet %d", i / 7);
		z *= 2;
		i /= 2;
	}
	printf("\n");
}
