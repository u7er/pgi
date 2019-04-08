#define MAXCOLORTC

#pragma pack (1)
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include "wingraph.h"

using namespace std;

struct  head {
	short bftype;
	long  bfsize;
	short rez1, rez2;
	long  bfoffbits;
	long  bisize;
	long  biwidth;
	long  biheight;
	short biplanes;
	short bibitcount;
	long  bicompression;
	long  bisizeimage;
	long  bix;
	long  biy;
	long  biclrused;
	long  biclrimp;
} head_file;

typedef struct TPaletteStruct
{
	int Red;
	int Green;
	int Blue;
} TPalette;

TPalette BMPPalette16[256];

unsigned char palitra[256][4];

int main() {
	FILE *f1, *f2;
	int n;
	int buffer[1024];

	f1 = fopen("_Carib256.bmp", "rb");

	n = fread(&head_file, sizeof(head_file), 1, f1);

	resize(1024, 768);

	for (int i = 0; i < (head_file.bfoffbits - sizeof(head_file)) / 4; i++) {
		n = fread(palitra[i], 4, 1, f1);
	}

	BMPPalette16[0].Red = palitra[0][0];
	BMPPalette16[0].Green = palitra[0][1];
	BMPPalette16[0].Blue = palitra[0][2];

	int count_palette = 1;

	for (int j = 0; j < 256; j++)
	{
		for (int k = 0; k < count_palette; k++)
		{
			int Delta = (palitra[j][0] - BMPPalette16[k].Red) * (palitra[0][0] - BMPPalette16[k].Red)
				+ (palitra[j][1] - BMPPalette16[k].Green) * (palitra[j][1] - BMPPalette16[k].Green)
				+ (palitra[j][2] - BMPPalette16[k].Blue) * (palitra[j][2] - BMPPalette16[k].Blue);

			if (Delta < 1600)
				break;

			if (k == count_palette - 1)
			{
				BMPPalette16[count_palette].Red = palitra[j][0];
				BMPPalette16[count_palette].Green = palitra[j][1];
				BMPPalette16[count_palette].Blue = palitra[j][2];
				count_palette++;
			}
		}
		if (count_palette == 16)
			break;
	}

	cout << "New size pallete BMP16: " << count_palette << endl << "Palette:" << endl;

	for (int i = 0; i < count_palette; i++)
	{
		cout << i << ") " << BMPPalette16[i].Red << " " << BMPPalette16[i].Green << " " << BMPPalette16[i].Blue << endl;
	}

	int y = head_file.biheight;
	int x = 0;

	do {
		n = fread(buffer, 1, 1, f1);

		int min = 100000;
		int index = 0;
		for (int k = 0; k < 16; k++)
		{
			int Delta = (palitra[buffer[0]][0] - BMPPalette16[k].Red) * (palitra[buffer[0]][0] - BMPPalette16[k].Red)
				+ (palitra[buffer[0]][1] - BMPPalette16[k].Green) * (palitra[buffer[0]][1] - BMPPalette16[k].Green)
				+ (palitra[buffer[0]][2] - BMPPalette16[k].Blue) * (palitra[buffer[0]][2] - BMPPalette16[k].Blue);

			if (Delta < min)
			{
				min = Delta;
				index = k;
			}
		}

		putpixel(x, y, RGB(BMPPalette16[index].Blue, BMPPalette16[index].Green, BMPPalette16[index].Red));

		x++;
		if (x == head_file.biwidth)
		{
			x = 0;
			y--;
		}

	} while (n != 0);

	fclose(f1);

}
