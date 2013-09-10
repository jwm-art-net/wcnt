#include <stdio.h>

// this program displays those numbers which are divisible by
// two, three, five, and seven.  Why ? - som'in' to do with
// triplets, quantuplets, and septuplets.

// lensel.cc is probably more useful

int main()
{

	int atwo, athree, afive, aseven;
	double btwo, bthree, bfive, bseven;
	
	for (int i = 2; i < 9999;i++)
	{
		atwo=i/2;
		athree=i/3;
		afive=i/5;
		aseven=i/7;
		btwo=i/(double)2;
		bthree=i/(double)3;
		bfive=i/(double)5;
		bseven=i/(double)7;
		
		if (atwo == btwo && athree==bthree && afive==bfive && aseven==bseven) 
			printf("\n%d",i);
	}
	return 0;
	
}