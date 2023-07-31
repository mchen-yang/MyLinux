#include "disp_manager.h"

/* 管理底层的LCD, WEB等 */
static PDispOper g_DispDevs = NULL;
static PDispOper g_DispDefault = NULL;
static DispBuff	 g_tDispBuff = NULL;
static unsigned int line_width;
static unsigned int pixel_width;

int PutPixel(int x, int y, unsigned int bp) {
	unsigned char* pen_8 = g_tDispBuff.buff + y * line_width + x * pixel_width;
	unsigned short* pen_16;
	unsigned int* pen_32;

	unsigned int red, green, blue;

	pen_16 = (unsigned short*)pen_8;
	pen_32 = (unsigned int*)pen_8;

	switch (g_tDispBuff.pB)
	{
	case 8:
	{
		*pen_8 = dwColor;
		break;
	}
	case 16:
	{
		/* 565 */
		red = (dwColor >> 16) & 0xff;
		green = (dwColor >> 8) & 0xff;
		blue = (dwColor >> 0) & 0xff;
		dwColor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
		*pen_16 = dwColor;
		break;
	}
	case 32:
	{
		*pen_32 = dwColor;
		break;
	}
	default:
	{
		printf("can't surport %dbpp\n", g_tDispBuff.pB);
		break;
	}
	}
	return 0;
}
void RegisterDisplay(PDispOper ptDispoper) {
	ptDispoper->ptNext = g_DispDevs;
	g_DispDevs = ptDispoper;
}

void DisplayInit(void) {
	FramebufferInit();
	//web
}
int SelectDefaultDisplay(char* name) {
	PDispOper pTemp = g_DispDevs;
	while (pTemp) {
		if (strcm(pTemp->name, name)) {
			g_DispDefault = pTemp;
			return 0;
		}
		pTemp = pTemp->ptNext;
	}
	return -1;
}

int FlushDefaultDisplay(void)(PRegion ptRegion, PDispBuff buffer) {
	return g_DispDefault->FlushRegion(ptRegion, buffer);
}

int InitDefaultDisplay(void) {
	int ret;
	ret = g_DispDefault->DeviceInit();
	if (ret) {
		printf("DeviceInit err\n");
		return -1;
	}
	ret = g_DispDefault->GetBuffer(g_tDispBuff);
	if (ret) {
		printf("GetBuffer err\n");
		return -1;
	}
	line_width = g_tDispBuff.pX * g_tDispBuff.pB / 8;
	pixel_width = g_tDispBuff.pB / 8;
	return 0;
}
