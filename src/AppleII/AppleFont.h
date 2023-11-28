#ifndef APPLE2_FONT_H
#define APPLE2_FONT_H

#include "Predef.h"


class AppleFont
{
private:
	unsigned char font[FONT_NUM][FONT_X*FONT_Y];
	unsigned char invfont[FONT_NUM][FONT_X*FONT_Y];
	//unsigned char* read_bmp(const char* fname, int* _w, int* _h);
	unsigned char* read_bmp_memory(char* buffer, int* _w, int* _h);
public:
	AppleFont();
	~AppleFont();

	void Create();
	void RenderFont(Color *backbuffer, int fontnum, int x, int y, bool inv);

};



#endif