#include <stdio.h>
#include "bitmap.h"
#include <time.h>

int main()
{
	word_t w = 0xBFFFFFFF;
	word_t p = 32;
	Bitmap bmp = makeBitmap();
	appendBits(bmp, 1, 3);
	appendBits(bmp, 0, 24);
	appendBits(bmp,1, 2);
	appendBits(bmp,0, 79);
	appendBits(bmp,1, 20);
	printBitmap(bmp);

	printf("%u\n", bmp->nbits);
	Bitmap bmpb = makeBitmap();
	/*
	appendBits(bmpb, 1, 31);
	appendBits(bmpb, 1, 31);
	appendWord(bmpb, 0x7C0001E0);
	appendWord(bmpb, 0x3FE00000);
	appendBits(bmpb, 0, 2);
	appendBits(bmpb, 1, 2);

	printBitmap(bmpb);


	Bitmap bmpd = c2_and(bmp, bmpb);
	printBitmap(bmpd);

	Bitmap bmde = c2_or(bmp, bmpb);
	printBitmap(bmde);
	
	
	appendBitmap(bmp, bmpb);
	printBitmap(bmp);
	
	Bitmap bmp4 = bmpand(bmp, bmpb);
	printBitmap(bmp4);

	decompress(bmp);
	decompress(bmpb);
	decompress(bmp4);
	printBitmap(bmp);
	printBitmap(bmpb);
	printBitmap(bmp4);
	*/
	const word_t bitnum = 100000;//10000000
	double density[10] = { 0.001, 0.00030, 0.001, 0.0035, 0.01, 0.02, 0.045, 0.1, 0.25, 0.5 };
	double time1[10] = { 0 };
	double time2[10] = { 0 };
	clock_t start, end,time;
	int i;
	Bitmap bmp1;// = makeBitmap();
	Bitmap bmp2;// = makeBitmap();
	//bmp = makeBitmap();
	//setBitmapSpace(bmp, bitnum / MAXBITS);




	for (i = 0; i < 10; i++)
	{
		randBitmap(bmp,bitnum,density[i]);
		randBitmap(bmpb,bitnum, density[i]);

		bmp1 = makeBitmap();
		bmp2 = makeBitmap();
		start = clock();
		bmp1 = c00_or(bmp, bmpb);
		end = clock();
		time1[i] = (double)(end - start) / CLOCKS_PER_SEC;
		
		compress(bmp);
		bitmapSize(bmp);
		compress(bmpb);
		bitmapSize(bmpb);
		
		bmp1 = makeBitmap();
		bmp2 = makeBitmap();
		start = clock();
		bmp2 = c2_or(bmp, bmpb);
		end = clock();
		time2[i] = (double)(end - start) / CLOCKS_PER_SEC;
		/*
		if (!isEqual(bmp1, bmp2))
		{
			printf("¼ÆËãÓÐÎó!!!\n");
			printBitmap(bmp);
			printBitmap(bmpb);
			printBitmap(bmp1);
			printBitmap(bmp2);
		}*/
	}

	for (i = 0; i < 10; i++)
		printf("%lf\t", time1[i]);
	printf("\n");
	for (i = 0; i < 10; i++)
		printf("%lf\t", time2[i]);

	return 0;
}
