#ifndef _DISP_MANAGER_H_
#define _DISP_MANAGER_H_

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
	char* GetBuffer(int* pX, int* pY, int* pB);
	

	int FlushRegion(PRegion ptRegion, char *buffer);
	struct DispOper* ptNext;
}DispOper, *PDispOper;

#endif // !_DISP_MANAGER_H_
