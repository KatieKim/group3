#include <cstdio>
#include <cstring>
#include <stdlib.h>
typedef struct{
	unsigned char r,g,b;
} RGB;


int main(void)
{
	unsigned char hdr[54];
	unsigned char *dat;
	int width, height, padding;
	int i,j;
	FILE *out, *fp = fopen("MARBLES.BMP","rb");
	RGB **img;

	fread(hdr,sizeof(unsigned char),54,fp);

	width = *(int*)&hdr[18];
	height = *(int*)&hdr[22];
	for(padding=0;(width*3+padding)%4;++padding);
	dat = (unsigned char*)malloc(sizeof(unsigned char)*((width*3)+padding));
	img = (RGB**)malloc(sizeof(RGB*)*width);
	for(i=0;i<width;++i)
		img[i] = (RGB*)malloc(sizeof(RGB)*height);
	/*====== 초기화 ======*/

	for(i=0;i<height;++i)
	{
		fread(dat,sizeof(unsigned char),width*3+padding,fp); /* 이미지는 bgr 순서로 저장됩니다. */
		for(j=0;j<width*3;j+=3)
		{
			img[j/3][height-i-1].b = dat[j];
			img[j/3][height-i-1].g = dat[j+1];
			img[j/3][height-i-1].r = dat[j+2];
		}
	}
	fclose(fp);
/*
	for(i=0;i<height;++i)
		for(j=0;j<width;++j)
		{
			int r = 0;
			int g = 0;
			int b = 0;
			for(int m=-1;m<2;++m)
				for(int n=-1;n<2;++n)
					if((i+m)>=0&&(i+m)<height&&(j+n)>=0&&(j+n)<width)
					{
						r+=img[j+n][i+m].b;
						g+=img[j+n][i+m].g;
						b+=img[j+n][i+m].r;
					}
			img[j][i].r = r/9;
			img[j][i].g = g/9;
			img[j][i].b = b/9;
		}
*/
	fp = fopen("MARBLES.BMP","rb");
	out = fopen("change.bmp","wb");
	fread(hdr,sizeof(unsigned char),54,fp);
	fwrite(hdr,sizeof(unsigned char),54,out);
	for(i=0;i<height;++i)
	{
		memset(dat,0,((width*3)+padding)*sizeof(unsigned char));
		for(j=0;j<width*3;j+=3)
		{
			//dat[j] = img[j/3][height-i-1].b;
			dat[j+1] = img[j/3][height-i-1].g;
			dat[j+2] = img[j/3][height-i-1].r;
		}
		fwrite(dat,sizeof(unsigned char),width*3+padding,out);
	}
	fclose(fp);
	return 0;
}
