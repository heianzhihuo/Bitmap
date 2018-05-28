#include "bitmap.h"

/*������bitmap����and*/
Bitmap bmpand(Bitmap bmpa, Bitmap bmpb)
{
	Bitmap tmp = makeBitmap();
	word_t nbitsa, nbitsb, nbits;
	nbitsa = bitmapSize(bmpa);
	nbitsb = bitmapSize(bmpb);
	if (nbitsa != nbitsb)
	{
		printf("��������С��ͬ��λ�������в���%u,%u\n", nbitsa, nbitsb);
	}
	if (nbitsa > nbitsb)
	{
		nbits = nbitsa;
		bitmapCopy(bmpb, tmp);
		adjustSize(tmp, nbitsa);
		return bmpand(bmpa, tmp);
	}
	else if (nbitsb > nbitsa)
	{
		nbits = nbitsa;
		bitmapCopy(bmpa, tmp);
		adjustSize(tmp, nbitsb);
		return bmpand(bmpb, tmp);
	}
	return c2_and(bmpa, bmpb);
}

/*������bitmap����or*/
Bitmap bmpor(Bitmap bmpa, Bitmap bmpb)
{
	Bitmap tmp = makeBitmap();
	word_t nbitsa, nbitsb, nbits;
	nbitsa = bitmapSize(bmpa);
	nbitsb = bitmapSize(bmpb);
	if (nbitsa != nbitsb)
	{
		printf("��������С��ͬ��λ�������в���%u,%u\n", nbitsa, nbitsb);
	}
	if (nbitsa > nbitsb)
	{
		nbits = nbitsa;
		bitmapCopy(bmpb, tmp);
		adjustSize(tmp, nbitsa);
		return bmpor(bmpa, tmp);
	}
	else if (nbitsb > nbitsa)
	{
		nbits = nbitsa;
		bitmapCopy(bmpa, tmp);
		adjustSize(tmp, nbitsb);
		return bmpand(bmpb, tmp);
	}
	return c2_or(bmpa, bmpb);
}

/*��δѹ����bitmap����and,����ѹ����bitmap*/
Bitmap c0_and(Bitmap bmpa, Bitmap bmpb)
{
	Bitmap result = makeBitmap();
	word_t i, j,wi,wj;
	for (i = 0; i < bmpa->m_size; i++)
	{
		wi = bmpa->m_vec[i];
		wj = bmpb->m_vec[i];
		appendWord(result, wi & wj);
	}
	if (bmpa->active.nbits)
	{
		result->active.val = bmpa->active.val & bmpb->active.val;
		result->active.nbits = bmpa->active.nbits;
	}
	return result;
}

/*��δѹ����bitmap����or������ѹ����bitmap*/
Bitmap c0_or(Bitmap bmpa, Bitmap bmpb)
{
	Bitmap result = makeBitmap();
	word_t i, j, wi, wj;
	for (i = 0; i < bmpa->m_size; i++)
	{
		wi = bmpa->m_vec[i];
		wj = bmpb->m_vec[i];
		appendWord(result, wi | wj);
	}
	if (bmpa->active.nbits)
	{
		result->active.val = bmpa->active.val | bmpb->active.val;
		result->active.nbits = bmpa->active.nbits;
	}
	return result;
}

/*������ѹ����bmp����and����Ҫ��bmp���ж���*/
Bitmap c2_and(Bitmap bmpa, Bitmap bmpb)
{
	Bitmap result = makeBitmap();
	Bitmap tmp = makeBitmap();
	word_t xi = 0, yi = 0;
	word_t x, y;
	word_t headx,heady;
	word_t countx = 0, county = 0;
	while (true)
	{
		if (countx == 0)
		{
			if (xi >= bmpa->m_size) break;
			x = bmpa->m_vec[xi];
			headx = x >> 30;
			countx = (~HEADER_1F)& x;
			xi++;
		}
		if (county == 0)
		{
			if (yi >= bmpb->m_size) break;
			y = bmpb->m_vec[yi];
			heady = y >> 30;
			county = (~HEADER_1F)& y;
			yi++;
		}
		if (headx < 2 && heady < 2)
		{
			//x,y����L,��ֱ�ӽ��л�ͬʱ���������豣��
			appendWord(result, x & y);
			countx = 0;
			county = 0;
		}
		else if (headx > 1 && heady > 1)
		{
			//x,y����fill����x��y����һ�����ľ�,�����С������0������С������1
			word_t count_min = countx > county ? county : countx;
			countx -= count_min;
			county -= count_min;
			if (headx == 3 && heady == 3)
				appendCounter(result, 1, count_min);//��������1-fill
			else
				appendCounter(result, 0, count_min);//������һ����0-fill
		}
		else if (headx > 1)
		{//x��fill
			countx--;//x����ʹ��
			county = 0;//y������һ��
			if (headx == 2)
				appendCounter(result, 0,1);//x��0-fill,���һ��0-fill
			else
				appendWord(result, y);//x��1-fill,���y
		}
		else
		{//y��fill
			county--;//y����ʹ��
			countx = 0;//x������һ��
			if (heady == 2)
				appendCounter(result, 0, 1);//y��0-fill,���һ��0-fill
			else
				appendWord(result, x);//y��1-fill,���x	
		}
	}
	if (xi != bmpa->m_size)
	{
		printf("ErrorX:%d\n", xi);
	}
	if (yi != bmpb->m_size)
	{
		printf("ErrorY:%d\n", yi);
	}
	if (bmpa->active.nbits)
	{//�Իλ���в���
		result->active.val = bmpa->active.val & bmpb->active.val;
		result->active.nbits = bmpa->active.nbits;
	}
	return result;
}

/*������ѹ����bmp����or����Ҫ��bmp���ж���*/
Bitmap c2_or(Bitmap bmpa, Bitmap bmpb)
{
	Bitmap result = makeBitmap();
	word_t xi = 0, yi = 0;
	word_t x, y;
	word_t headx, heady;
	word_t countx = 0, county = 0;
	while (true)
	{
		if (countx == 0)
		{
			if (xi >= bmpa->m_size) break;
			x = bmpa->m_vec[xi];
			headx = x >> 30;
			countx = (~HEADER_1F)& x;
			xi++;
		}
		if (county == 0)
		{
			if (yi >= bmpb->m_size) break;
			y = bmpb->m_vec[yi];
			heady = y >> 30;
			county = (~HEADER_1F)& y;
			yi++;
		}
		if (headx < 2 && heady < 2)
		{
			//x,y����L,��ֱ�ӽ��л�ͬʱ���������豣��
			appendWord(result, x | y);
			countx = 0;
			county = 0;
		}
		else if (headx > 1 && heady > 1)
		{
			//x,y����fill����x��y����һ�����ľ�,�����С������0������С������1
			word_t count_min = countx > county ? county : countx;
			countx -= count_min;
			county -= count_min;
			if (headx == 3 || heady == 3)
				appendCounter(result, 1, count_min);//������һ����1-fill
			else
				appendCounter(result, 0, count_min);//��������0-fill
		}
		else if (headx > 1)
		{//x��fill
			countx--;//x����ʹ��
			county = 0;//y������һ��
			if (headx == 3)
				appendCounter(result, 1, 1);//x��1-fill,���һ��1-fill
			else
				appendWord(result, y);//x��0-fill,���y
		}
		else
		{//y��fill
			county--;//y����ʹ��
			countx = 0;//x������һ��
			if (heady == 3)
				appendCounter(result, 1, 1);//y��1-fill,���һ��1-fill
			else
				appendWord(result, x);//y��0-fill,���x	
		}
	}
	if (xi != bmpa->m_size)
	{
		printf("ErrorX:%d\n", xi);
	}
	if (yi != bmpb->m_size)
	{
		printf("ErrorY:%d\n", yi);
	}
	if (bmpa->active.nbits)
	{
		result->active.val = bmpa->active.val | bmpb->active.val;
		result->active.nbits = bmpa->active.nbits;
	}
	return result;
}

/*����bitmap�е���Чλ��*/
word_t bitmapSize(Bitmap bmp)
{
	return bmp->nbits + bmp->active.nbits;
}

/*��bitmap����WAHѹ��*/
void compress(Bitmap bmp)
{
	word_t *tmp;//ѹ��ʱ������ʱ��Ŵ�
	tmp = (word_t *)palloc(sizeof(word_t)*bmp->m_size);
	word_t i,j;//i,j�ֱ��ǵ�ǰ����ĺ��µ��������ڴ����word���±�
	word_t w;//w�Ǿɵ�word
	int headi, headj;//headi,��ǰ���ڴ����head,headj,��һ�δ����
	for (i = 0,j = 0; i < bmp->m_size; i++)
	{
		w = bmp->m_vec[i];
		if (w == 0)
		{
			//ȫ0
			headi = 2;
			w = HEADER_0F | 1;
		}
		else if (w == ALLONES)
		{
			//ȫ1
			headi = 3;
			w = HEADER_1F | 1;
		} 
		else
			headi = w >> 30;
		if (j == 0)
		{
			tmp[j] = w;
			j++;
		}
		else
		{
			if (headi == headj)
			{
				//������ͬ
				tmp[j] += w & (~HEADER_1F);
			}
			else
			{
				tmp[j] = w;
				j++;
			}
		}
		headj = headi;
	}
	bmp->m_size = j;
	free(bmp->m_vec);
	bmp->m_vec = tmp;
}

/*��bitmap���н�ѹ���������з��鴦����ѹ��ÿ��word 31λ,���λΪ0*/
void decompress(Bitmap bmp)
{
	word_t *tmp;//ѹ��ʱ������ʱ��Ŵ�
	tmp = (word_t *)palloc(sizeof(word_t)*(bmp->nbits/MAXBITS));
	word_t i, j, k, w, cnt;
	int head;
	for (i = 0,j=0; i < bmp->m_size; i++)
	{
		w = bmp->m_vec[i];
		head = w >> 30;
		cnt = (~HEADER_1F)&w;
		if (head > 1)
		{//fill
			if (head == 2)//0-fill
				for (k = 0; k < cnt; k++, j++)
					tmp[j] = 0;
			else//1-fill
				for (k = 0; k < cnt; k++, j++)
					tmp[j] = ALLONES;
		}
		else
		{//Literal
			tmp[j] = w;
			j++;
		}
	}
	bmp->m_size = j;
	free(bmp->m_vec);
	bmp->m_vec = tmp;
}

/*16�������bitmap*/
void printBitmap(Bitmap bmp)
{
	word_t i;
	for (i = 0; i < bmp->m_size; i++)
		printf("%08x ", bmp->m_vec[i]);
	if (bmp->active.nbits)
		printf("%08x", bmp->active.val);
	printf("\n");
}

/*��a copy��b*/
void bitmapCopy(Bitmap bmpa, Bitmap bmpb)
{
	if (bmpb == NULL)
		bmpb = makeBitmap();
	bmpb->nbits = bmpa->nbits;
	bmpb->active = bmpa->active;
	bmpb->m_size = bmpa->m_size;
	bmpb->isCompressed = bmpa->isCompressed;
	setBitmapSpace(bmpb, bmpa->m_size);
	memcpy(bmpb->m_vec, bmpa->m_vec, bmpa->m_size * sizeof(word_t));
}

/*����bitmap����Чλ����ʹλ���ﵽnbits��ͨ�����Ҳ����0*/
void adjustSize(Bitmap bmp, word_t nbits)
{
	word_t sz = bmp->nbits + bmp->active.nbits;
	if (sz == nbits) return;
	if (sz < nbits)
		appendBits(bmp, 0, nbits - sz);
}

/*����һ���յ�bitmap*/
Bitmap makeBitmap()
{
	Bitmap bmp = (Bitmap)palloc(sizeof(bitmap));
	bmp->active.nbits = 0;
	bmp->active.val = 0;
	bmp->m_vec = NULL;
	bmp->isCompressed = false;
	bmp->m_size = 0;
	bmp->nbits = 0;
	return bmp;
}

/*��active wordΪ��ʱ�����һ��counterΪcnt��val-fill
 * �������cnt��val-fill
 * val������0����1
 */
void appendCounter(Bitmap bmp, int val, word_t cnt)
{
	word_t head = 2 + val;
	word_t w = (head << 30) | cnt;//����WAH word
	if (isEmpty(bmp))
		pushBitmap(bmp, w);//ֱ�����
	else
	{
		word_t w1 = getBitmapTop(bmp);
		if (w1 >> 30 == head)
			setBitmapTop(bmp, w1 + cnt);//��top��counter��cnt
		else
			pushBitmap(bmp, w);//��w��ӵ��洢�ռ�
	}
	bmp->nbits += cnt * MAXBITS;//����λ��
}

/* 
 * ����31λ��active word��ӵ�bitmap��,
 * ͬʱ���active word
 * active word��δѹ����
 */
void appendActive(Bitmap bmp)
{
	word_t w = bmp->active.val;
	word_t head;
	if (w == 0)
	{
		//ȫ0
		head = 2;
		w = HEADER_0F | 1;
	}
	else if (w == ALLONES)
	{
		//ȫ1
		head = 3;
		w = HEADER_1F | 1;
	}
	else
		head = 0;
	if (isEmpty(bmp))
		pushBitmap(bmp, w);//��w��ӵ��洢�ռ�
	else
	{
		word_t w1 = getBitmapTop(bmp);
		if (w1 >> 30 == head && head >1)//��top��word����ͬ���͵�fill
			setBitmapTop(bmp, w1 + 1);//top��counter��һ
		else pushBitmap(bmp, w);//��w��ӵ��洢�ռ�
	}
	bmp->nbits += MAXBITS;//���Ӵ洢λ��
	bmp->active.nbits = 0;
	bmp->active.val = 0;
}

/*��һ��bitmap��ӵ���һ��bitmap��*/
void appendBitmap(Bitmap bmp, Bitmap bmpa)
{
	word_t ex_size = bitmapSize(bmp) + bitmapSize(bmpa);
	word_t i;
	for (i = 0; i < bmpa->m_size; i++)
		appendWord(bmp,bmpa->m_vec[i]);//��a�е�word��ӵ�bmp��
	if (bmpa->active.nbits)
	{//�ϲ�active word
		if (bmp->active.nbits + bmpa->active.nbits < MAXBITS)
		{
			//�ϲ�����31λ
			bmp->active.val << bmpa->active.nbits;//�ճ�λ��
			bmp->active.val |= bmpa->active.val;
			bmp->active.nbits = bmp->active.nbits + bmpa->active.nbits;
		}
		else
		{
			//�ϲ��󳬹�31λ
			word_t nb1 = bmp->active.nbits + bmpa->active.nbits - MAXBITS;//��������
			bmp->active.val << (MAXBITS - bmp->active.nbits);//�ڳ���λ
			word_t w1 = bmpa->active.val >> nb1;//��ȡ��b����Ӧλ��
			bmp->active.val |= w1;//�ϲ���31λ
			appendActive(bmp);
			bmp->active.nbits = nb1;//
			if (nb1 > 0)
				bmp->active.val = ((1U << nb1) - 1) & bmpa->active.val;//ʣ�ಿ��
		}
	}
	else
	{//�򵥵ĸ��ƹ�ȥ
		bmp->active.nbits = bmpa->active.nbits;
		bmp->active.val = bmpa->active.val;
	}
	if (ex_size != bitmapSize(bmp))
	{
		printf("Error");
	}
}

/*�޸�bitmapջ���������wordΪw*/
void setBitmapTop(Bitmap bmp, word_t w)
{
	if (isEmpty(bmp)) return;
	bmp->m_vec[bmp->m_size - 1] = w;
}

/*���bitmapջ������word*/
word_t getBitmapTop(Bitmap bmp)
{
	if (isEmpty(bmp))
		return 0;
	return bmp->m_vec[bmp->m_size - 1];
}

/*bitmap�Ƿ�Ϊ��*/
bool isEmpty(Bitmap bmp)
{
	return !bmp->m_size;
}

/*��bitmap�Ĵ洢�ռ������һ��word*/
void pushBitmap(Bitmap bmp, word_t w)
{
	resetBitmapSpace(bmp, bmp->m_size + 1);
	bmp->m_vec[bmp->m_size] = w;
	bmp->m_size++;
}

/*��bitmap�����һ��word w��w���λΪ1��ʾ��ѹ��*/
void appendWord(Bitmap bmp, word_t w)
{
	word_t w1;
	int head = w >> 30;
	int b2 = head % 2;
	int nb1, nb2;//nb1��nb2,���ڱ���λ��
	word_t cnt;
	if (bmp->active.nbits)
	{
		nb1 = bmp->active.nbits;
		nb2 = MAXBITS - nb1;
		bmp->active.val = bmp->active.val << nb2;
		if (head > 1)
		{
			//��ѹ��
			if (head == 3)
			{
				//�����1-fill��active��λӦ����1
				w1 = (1 << nb2) - 1;
				bmp->active.val |= w1;
			}
			appendActive(bmp);
			cnt = (w & (~HEADER_1F)) - 1;//counter
			if (cnt>0)
				appendCounter(bmp, b2, cnt);
			bmp->active.nbits = nb1;
			bmp->active.val = ((1 << nb1) - 1)*b2;
		}
		else
		{
			//δѹ��
			w1 = (w >> nb1);
			bmp->active.val |= w1;
			appendActive(bmp);
			w1 = (1 << nb1) - 1;
			bmp->active.val = w&w1;//activeȡw�е�nbitsλ
			bmp->active.nbits = nb1;//�����31λ��nbits���䣬
		}
	}
	else
	{
		//û��active
		if (head > 1)
		{
			//ѹ��
			cnt = w & (~HEADER_1F);//counter
			if (cnt>0)
				appendCounter(bmp, b2, cnt);
		}
		else
		{
			//δѹ��
			bmp->active.val = w;
			appendActive(bmp);
		}
	}
}

/*
 * ��bitmap�����n��0����1
 * val������1����0
 */
void appendBits(Bitmap bmp, word_t val, word_t n)
{
	if (n == 0) return;
	if (bmp->active.nbits>0)
	{
		//��active������active����ӣ���
		word_t tmp = MAXBITS - bmp->active.nbits;
		if (tmp>n) tmp = n;
		bmp->active.nbits += tmp;
		bmp->active.val <<= tmp;
		n -= tmp;
		if (val == 1)
			bmp->active.val |= (1U << tmp) - 1;
		if (bmp->active.nbits == MAXBITS)
			appendActive(bmp);
	}
	if (n >= MAXBITS)
	{
		//ʣ��ĳ���31λ
		word_t cnt = n / MAXBITS;
		appendCounter(bmp, val, cnt);
		n -= cnt * MAXBITS;//��31λһ����Ӻ�ʣ���
	}
	if (n>0)
	{
		//ʣ�಻��31λ��
		bmp->active.nbits = n;
		bmp->active.val = val*((1U << n) - 1);
	}
}

/*��bitmap�����һλval��val������0��1*/
void appendOneBit(Bitmap bmp, word_t val)
{
	word_t w = bmp->active.val;
	bmp->active.val = (bmp->active.val << 1) | val;
	bmp->active.nbits++;
	if (bmp->active.nbits == MAXBITS)
		appendActive(bmp);
}

/*
 * ��bitmap��ջ����洢�ռ�
 * size��ʾword��
 */
void setBitmapSpace(Bitmap bmp, word_t size)
{
	bmp->m_vec = (word_t *)palloc(size * sizeof(word_t));
	memset(bmp->m_vec, 0, size * sizeof(word_t));
}

/*
 * ����һ��bitmap�Ĵ洢�ռ䣬
 * ����ԭ���ݸ��Ƶ��µĿռ���
 * ͬʱ�ͷ�ԭ�пռ�
 */
void resetBitmapSpace(Bitmap bmp, word_t size)
{
	word_t *w = (word_t *)palloc(size * sizeof(word_t));
	memcpy(w, bmp->m_vec, bmp->m_size * sizeof(word_t));
	free(bmp->m_vec);
	bmp->m_vec = w;
}
