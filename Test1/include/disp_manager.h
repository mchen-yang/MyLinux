#ifndef _DISP_MANAGER_H_
#define _DISP_MANAGER_H_

#include <common.h>
#include <font_manager.h>

typedef struct DispBuff {
	int pX;
	int pY;
	int pB;
	char* buff;
}DispBuff, *PDispBuff;

typedef struct DispOper {
	char* name;
	int (*DeviceInit)(void);
	int (*DeviceExit)(void);
	int (*GetBuffer)(PDispBuff ptDispBuff);
	int (*FlushRegion)(PRegion ptRegion, PDispBuff buffer);
	struct DispOper* ptNext;
}DispOper, *PDispOper;
void DrawTextInRegionCentral(char *name, PRegion ptRegion, unsigned int dwColor);

void DrawFontBitMap(PFontBitMap ptFontBitMap, unsigned int dwColor);
void DrawRegion(PRegion ptRegion, unsigned int dwColor);

int PutPixel(int x, int y, unsigned int bp);
void RegisterDisplay(PDispOper ptDispoper);
void DisplaySystemRegister(void);
int SelectDefaultDisplay(char* name);
int FlushDefaultDisplay(PRegion ptRegion, PDispBuff buffer);
int InitDefaultDisplay(void);
PDispBuff GetDispBuff(void);



#endif // !_DISP_MANAGER_H_
