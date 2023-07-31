#include <stdio.h>
#include <string.h>

#include <input_manager.h>

/* 管理底层的LCD, WEB等 */
static PInputOper g_InputDevs = NULL;
static PInputOper g_InputDefault = NULL;
static InputData	 g_tInputData;


void RegisterInput(PInputOper ptInputOper) {
	ptInputOper->ptNext = g_InputDevs;
	g_InputDevs = ptInputOper;
}

void InputInit(void) {
/*TODO: Initialize*/
//	void TouchScreenInit();
//
//	TouchScreenInit();
	//web
}

int SelectDefaultInput(char* name) {
	PInputOper pTemp = g_InputDevs;
	while (pTemp) {
		if (strcmp(pTemp->name, name) == 0) {
			g_InputDefault = pTemp;
			return 0;
		}
		pTemp = pTemp->ptNext;
	}
	return -1;
}

int FlushDefaultDisplay(PRegion ptRegion, PDispBuff buffer) {
	return g_DispDefault->FlushRegion(ptRegion, buffer);
}

int InitDefaultDisplay(void) {
	int ret;
	ret = g_DispDefault->DeviceInit();
	if (ret) {
		printf("DeviceInit err\n");
		return -1;
	}
	ret = g_DispDefault->GetBuffer(&g_tDispBuff);
	if (ret) {
		printf("GetBuffer err\n");
		return -1;
	}
	line_width = g_tDispBuff.pX * g_tDispBuff.pB / 8;
	pixel_width = g_tDispBuff.pB / 8;
	return 0;
}

PDispBuff GetDispBuff(void){
	return &g_tDispBuff;
}



