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
} BmpHeader;

struct PaletteStruct {
	int Red;
	int Green;
	int Blue;
} Palette;

Palette Bmp16Palette[256];

unsigned char palitra[256][4];

int main() {
	FILE *bmp256s, *bmp16d;
	int n;
	int buffer[1024];

	bmp256s = fopen("_Carib256.bmp", "rb");

	n = fread(&BmpHeader, sizeof(BmpHeader), 1, bmp256s);

	resize(1024, 768);

	for (int i = 0; i < (BmpHeader.bfoffbits - sizeof(BmpHeader)) / 4; i++) {
		n = fread(palitra[i], 4, 1, bmp256s);
	}

	Bmp16Palette[0].Red = palitra[0][0];
	Bmp16Palette[0].Green = palitra[0][1];
	Bmp16Palette[0].Blue = palitra[0][2];

	int paletteCount = 1;

	for (int j = 0; j < 256; j++){
		for (int k = 0; k < paletteCount; k++){
			int Delta = (palitra[j][0] - Bmp16Palette[k].Red) * (palitra[0][0] - Bmp16Palette[k].Red)
				+ (palitra[j][1] - Bmp16Palette[k].Green) * (palitra[j][1] - Bmp16Palette[k].Green)
				+ (palitra[j][2] - Bmp16Palette[k].Blue) * (palitra[j][2] - Bmp16Palette[k].Blue);

			if (Delta < 1600)
				break;

			if (k == paletteCount - 1){
				Bmp16Palette[paletteCount].Red = palitra[j][0];
				Bmp16Palette[paletteCount].Green = palitra[j][1];
				Bmp16Palette[paletteCount].Blue = palitra[j][2];
				paletteCount++;
			}
		}
		if (paletteCount == 16)
			break;
	}

	cout << "Size of palette for BMP 16-bit: " << paletteCount 
		<< endl << "Palette array:" << endl;

	for (int i = 0; i < paletteCount; i++){
		cout << Bmp16Palette[i].Red << " " 
			<< Bmp16Palette[i].Green << " " 
			<< Bmp16Palette[i].Blue << endl;
	}

	int y = BmpHeader.biheight;
	int x = 0;

	do {
		n = fread(buffer, 1, 1, bmp256s);

		if (n == 0) return -22;

		int min = 100000;
		int index = 0;
		for (int k = 0; k < 16; k++){
			int Delta = (palitra[buffer[0]][0] - Bmp16Palette[k].Red) * (palitra[buffer[0]][0] - Bmp16Palette[k].Red)
				+ (palitra[buffer[0]][1] - Bmp16Palette[k].Green) * (palitra[buffer[0]][1] - Bmp16Palette[k].Green)
				+ (palitra[buffer[0]][2] - Bmp16Palette[k].Blue) * (palitra[buffer[0]][2] - Bmp16Palette[k].Blue);

			if (Delta < min){
				min = Delta;
				index = k;
			}
		}

		putpixel(x, y, RGB(Bmp16Palette[index].Blue, Bmp16Palette[index].Green, Bmp16Palette[index].Red));

		x++;
		if (x == BmpHeader.biwidth){
			x = 0;
			y--;
		}

	} while (n != 0);

	fclose(bmp256s);

}
