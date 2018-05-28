#pragma once
#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED

#include <stdio.h>
#include <xmmintrin.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#endif // BITMAP_H_INCLUDED

#define palloc(n) malloc(n)
typedef uint32_t uint32;

typedef uint32 word_t;

static const word_t WORD_SIZE = 32;
static const word_t MAXBITS = 31;
static const word_t HEADER_0F = 2U << 30;
static const word_t HEADER_1F = 3U << 30;
static const word_t ALLONES = ~(1 << 31);

typedef struct
{
	word_t val;//ԭʼ����,�������Ҳ�
	word_t nbits;//λ������Чλ��,���λ��ʼ����
}ActiveWord;//�word������31λ��word

/* bitmap �ṹ */
typedef struct
{
	bool isCompressed;
	word_t nbits;//λ����������λ�����������nbits�������word��nbits
	word_t *m_vec;//�洢�ռ�
	word_t m_size;//�洢�ռ��С
	ActiveWord active;//�word
}bitmap;
typedef bitmap *Bitmap;

Bitmap makeBitmap();
bool isEmpty(Bitmap bmp);
void setBitmapSpace(Bitmap bmp, word_t size);
void resetBitmapSpace(Bitmap bmp, word_t size);
void setBitmapTop(Bitmap bmp, word_t w);
word_t getBitmapTop(Bitmap bmp);
void pushBitmap(Bitmap bmp, word_t w);
void appendActive(Bitmap bmp);
void appendCounter(Bitmap bmp, int val, word_t cnt);
word_t bitmapSize(Bitmap bmp);
void bitmapCopy(Bitmap bmpa, Bitmap bmpb);
void adjustSize(Bitmap bmp, word_t nbits);
void printBitmap(Bitmap bmp);

void appendWord(Bitmap bmp, word_t w);
void appendOneBit(Bitmap bmp, word_t val);
void appendBits(Bitmap bmp, word_t val, word_t n);
void appendBitmap(Bitmap bmp, Bitmap bmpa);


void compress(Bitmap bmp);
void decompress(Bitmap bmp);





/*��bitmap.c��ʵ��*/

Bitmap bmpand(Bitmap bmpa, Bitmap bmpb);
Bitmap bmpor(Bitmap bmpa, Bitmap bmpb);

Bitmap c2_or(Bitmap bmpa, Bitmap bmpb);
Bitmap c1_or(Bitmap bmpa, Bitmap bmpb);
Bitmap c0_or(Bitmap bmpa, Bitmap bmpb);
Bitmap c2_and(Bitmap bmpa, Bitmap bmpb);
Bitmap c1_and(Bitmap bmpa, Bitmap bmpb);
Bitmap c0_and(Bitmap bmpa, Bitmap bmpb);




