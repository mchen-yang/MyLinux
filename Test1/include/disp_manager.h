#ifndef _DISP_MANAGER_H_
#define _DISP_MANAGER_H_

typedef struct DispBuff {
	int* pX;
	int* pY;
	int* pB;
	char* buff;
}DispBuff, *PDispBuff;

typedef struct Region {
	int LeftUpX;
	int LeftUpY;
	int Width;
	int Heigh;
}Region, *PRegion;

typedef struct DispOper {
	char* name;
	int DeviceInit(void);
	int DeviceExit(void);
	int GetBuffer(PDispBuff ptDispBuff);
	int FlushRegion(PRegion ptRegion, PDispBuff buffer);
	struct DispOper* ptNext;
}DispOper, *PDispOper;

int PutPixel(int x, int y, unsigned int bp);
void RegisterDisplay(PDispOper ptDispoper);
void DisplayInit(void);
int SelectDefaultDisplay(char* name);
int FlushDefaultDisplay(void)(PRegion ptRegion, PDispBuff buffer);
int InitDefaultDisplay(void);
#endif // !_DISP_MANAGER_H_
