#include <stdio.h>
#include <string.h>

#include <disp_manager.h>

/* ¹ÜÀíµ×²ãµÄLCD, WEBµÈ */
static PDispOper g_DispDevs = NULL;
static PDispOper g_DispDefault = NULL;
static DispBuff	 g_tDispBuff;
static unsigned int line_width;
static unsigned int pixel_width;

int PutPixel(int x, int y, unsigned int dwColor) {
	unsigned char* pen_8 = (unsigned char*)g_tDispBuff.buff + y * line_width + x * pixel_width;
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
			return -1;
			break;
		}
	}
	return 0;
}

void RegisterDisplay(PDispOper ptDispOper) {
	ptDispOper->ptNext = g_DispDevs;
	g_DispDevs = ptDispOper;
}

void DisplaySystemRegister(void) {
	void FramebufferRegister();

	FramebufferRegister();
	//web
}

int SelectDefaultDisplay(char* name) {
	PDispOper pTemp = g_DispDevs;
	while (pTemp) {
		if (strcmp(pTemp->name, name) == 0) {
			g_DispDefault = pTemp;
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

void DrawTextInRegionCentral(char *name, PRegion ptRegion, unsigned int dwColor){
//	int n = strlen(name);
//	int iFontSize = ptRegion->Width / n / 2;
	FontBitMap tFontBitMap;
	RegionCartesian tRegionCar;

	int i=0;
	int iOriginX, iOriginY;
	int error;
		/* è®¡ç®—å­—ç¬¦ä¸²çš„å¤–æ¡† */
	GetStringRegionCar(name, &tRegionCar);
	
	/* ç®—å‡ºç¬¬ä¸€ä¸ªå­—ç¬¦çš„origin */
	iOriginX = ptRegion->LeftUpX + (ptRegion->Width - tRegionCar.Width)/2 - tRegionCar.LeftUpX;
	iOriginY = ptRegion->LeftUpY + (ptRegion->Heigh - tRegionCar.Heigh)/2 + tRegionCar.LeftUpY;


//	if (iFontSize > ptRegion->Heigh)
//		iFontSize =  ptRegion->Heigh;
//
//	iOriginX = (ptRegion->Width - n * iFontSize)/2 + ptRegion->LeftUpX;
//	iOriginY = (ptRegion->Heigh - iFontSize)/2 + iFontSize + ptRegion->LeftUpY;

//	SetFontSize(iFontSize);//TODO:why

	while (name[i])
	{
		/* get bitmap */
		tFontBitMap.iCurOriginX = iOriginX;
		tFontBitMap.iCurOriginY = iOriginY;
		error = GetFontBitMap(name[i], &tFontBitMap);
		if (error)
		{
			printf("DrawTextInRegionCentral err\n");
			return;
		}

		/* draw on buffer */
		DrawFontBitMap(&tFontBitMap, dwColor);

		iOriginX = tFontBitMap.iNextOriginX;
		iOriginY = tFontBitMap.iNextOriginY;	
		i++;

	}
}

void DrawFontBitMap(PFontBitMap ptFontBitMap, unsigned int dwColor)
{
  int i, j, p, q;
	int x = ptFontBitMap->tRegion.LeftUpX;
	int y = ptFontBitMap->tRegion.LeftUpY;
  int x_max = x + ptFontBitMap->tRegion.Width;
  int y_max = y + ptFontBitMap->tRegion.Heigh;
	int width = ptFontBitMap->tRegion.Width;
	unsigned char *buffer = ptFontBitMap->pucBuffer;

    //printf("x = %d, y = %d\n", x, y);

    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
        for ( i = x, p = 0; i < x_max; i++, p++ )
        {
            if ( i < 0      || j < 0       ||
                i >= g_tDispBuff.pX || j >= g_tDispBuff.pY )
            continue;

            //image[j][i] |= bitmap->buffer[q * bitmap->width + p];
            if (buffer[q * width + p])
	            PutPixel(i, j, dwColor);
        }
    }
	
}
void DrawRegion(PRegion ptRegion, unsigned int dwColor){
	int x = ptRegion->LeftUpX;
	int y = ptRegion->LeftUpY;
	int width = ptRegion->Width;
	int heigh = ptRegion->Heigh;
	int j,i;
	for (j = y; j < y + heigh; j++)
	{
		for (i = x; i < x + width; i++)
			PutPixel(i, j, dwColor);
	}

	
}

