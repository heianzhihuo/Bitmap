#include <stdio.h>
#include "bitmap.h"

int main()
{
	word_t w = 0xBFFFFFFF;
	word_t p = 32;
	Bitmap bmp = makeBitmap();
	appendOneBit(bmp, 1);
	appendBits(bmp, 0, 20);
	appendBits(bmp,1, 3);
	appendBits(bmp,0, 79);
	appendBits(bmp,1, 25);
	printBitmap(bmp);

	printf("%u\n", bmp->nbits);
	Bitmap bmpb = makeBitmap();
	appendBits(bmpb, 1, 31);
	appendBits(bmpb, 1, 31);
	appendWord(bmpb, 0x7C0001E0);
	appendWord(bmpb, 0x3FE00000);
	appendBits(bmpb, 0, 2);
	appendBits(bmpb, 1, 2);

	printBitmap(bmpb);


	Bitmap bmpd = c2_and(bmp, bmpb);
	printBitmap(bmpd);
	
	
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

	return 0;
}
