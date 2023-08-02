#ifndef _FONT_MANAGER_H
#define _FONT_MANAGER_H

#include <sys/time.h>
#include <common.h>
#include <string.h>

typedef struct FontBitMap {
	Region tRegion;
	int iCurOriginX;
	int iCurOriginY;
	int iNextOriginX;
	int iNextOriginY;
	unsigned char *pucBuffer;//TODO
}FontBitMap, *PFontBitMap;

typedef struct FontOpr{
	char *name;
	int (*FontInit)(char *aFineName);//TODO: which font file
	int (*SetFontSize)(int iFontSize);
	int (*GetFontBitMap)(unsigned int dwCode, PFontBitMap ptFontBitMap);//TODO:ascii AND BITMAP

	struct FontOpr* ptNext;
}FontOpr, *PFontOpr;

void FontsRegister(PFontOpr ptFontOper);

void FontInit(void);

int SelectAndInitFont(char *aFontOprName, char *aFontFileName);

int SetFontSize(int iFontSize);

int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap);


#endif




