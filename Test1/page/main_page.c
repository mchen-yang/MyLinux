#include <config.h>
#include <page_manager.h>
#include <disp_manager.h>
#include <input_manager.h>
#include <ui.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define X_GAP 5
#define Y_GAP 5

static Button g_tButtons[ITEMCFG_MAX_NUM];
static int g_tButtonCnt;

static int MainPageOnPressed(struct Button *ptButton,  PDispBuff ptDispBuff, PInputData ptInputData)
{
	unsigned int dwColor = BUTTON_DEFAULT_COLOR;
	char name[100];
	char status[100];
	char *strButton;

	strButton = ptButton->name;
	/* Touch: */
	if(ptInputData->InputType == INPUT_TYPE_TOUCH && (ptInputData->Pressure)){
		/* is bCanBeTouched*/
		if(GetItemCfgByName(ptButton->name)->bCanBeTouched == 0){
			return -1;
		}
		
		/* Touch handle */
		ptButton->status = !ptButton->status;
		if (ptButton->status)
			dwColor = BUTTON_PRESSED_COLOR;
	}
	/* Net: */
	else if(ptInputData->InputType == INPUT_TYPE_NET ){
		/*  */

		/* InputStr : wifi ok, wifi err, burn 70 */
		sscanf(ptInputData->InputStr, "%s %s", name, status);

		if(strcmp(status, "ok") == 0){
			dwColor = BUTTON_PRESSED_COLOR;
		}
		else if(strcmp(status, "err") == 0){
			dwColor = BUTTON_DEFAULT_COLOR;
		}
		else if(status[0] >= '0' && status[0] <= '9'){//TODO
			dwColor = BUTTON_PERCENT_COLOR;
			strButton = status;		
		}
		else{
			return -1;
		}
	}
	else {
		return -1;
	}

	DrawRegion(&ptButton->tRegion, dwColor);

	/* 居中写文字 */
	DrawTextInRegionCentral(strButton, &ptButton->tRegion, BUTTON_TEXT_COLOR );

	/* flush to lcd/web */
	FlushDefaultDisplay(&ptButton->tRegion, ptDispBuff);

	return 0;
}

static void GenerateButton(void){
	int width, height;
	int n_per_line;
	int row, rows;
	int col;
	int n;
	PDispBuff pDispBuff;
	int xres, yres;
	int start_x, start_y;
	int pre_start_x, pre_start_y;
	PButton pButton;
	int i = 0;
	
	/* caculate width and height */
	n = GetItemCfgCount();
	g_tButtonCnt = n;								//TODO
	pDispBuff = GetDispBuff();
	xres = pDispBuff->pX;
	yres = pDispBuff->pY;
	width = sqrt(1.0/0.618 *xres * yres / n);
	n_per_line = xres / width + 1;
	width  = xres / n_per_line;
	height = 0.618 * width;	

	/* centralize */
	start_x = (xres - width * n_per_line) / 2;
	rows    = n / n_per_line;
	if (rows * n_per_line < n)
		rows++;
	
	start_y = (yres - rows*height)/2;


	/* region */
	for (row = 0; (row < rows) && (i < n); row++)
	{
		pre_start_y = start_y + row * height;
		pre_start_x = start_x - width;
		for (col = 0; (col < n_per_line) && (i < n); col++)
		{
			pButton = &g_tButtons[i];
			pButton->tRegion.LeftUpX = pre_start_x + width;
			pButton->tRegion.LeftUpY = pre_start_y;
			pButton->tRegion.Width   = width - X_GAP;
			pButton->tRegion.Heigh   = height - Y_GAP;
			pre_start_x = pButton->tRegion.LeftUpX;

			/* InitButton */
			InitButton(pButton, GetItemCfgByIndex(i)->name, NULL, NULL, MainPageOnPressed);
			i++;
		}
	}

	/* OnDraw */
		for (i = 0; i < n; i++)
		g_tButtons[i].OnDraw(&g_tButtons[i], pDispBuff);
}

static int isTouchPointInRegion(int iX, int iY, PRegion ptRegion)
{
	if (iX < ptRegion->LeftUpX || iX >= ptRegion->LeftUpX + ptRegion->Width)
		return 0;

	if (iY < ptRegion->LeftUpY || iY >= ptRegion->LeftUpY + ptRegion->Heigh)
		return 0;

	return 1;
}
static PButton GetButtonByName(char *name)
{
	int i;
	
	for (i = 0; i < g_tButtonCnt; i++)
	{
		if (strcmp(name, g_tButtons[i].name) == 0)
			return &g_tButtons[i];
	}

	return NULL;
}

static PButton GetButtonByInputEvent(PInputData ptData)
{
	int i;
	char name[100];
	
	if (ptData->InputType == INPUT_TYPE_TOUCH)
	{
		for (i = 0; i < g_tButtonCnt; i++)
		{
			if (isTouchPointInRegion(ptData->X, ptData->Y, &g_tButtons[i].tRegion))
				return &g_tButtons[i];
		}
	}
	else if (ptData->InputType == INPUT_TYPE_NET)
	{
		sscanf(ptData->InputStr, "%s", name);
		return GetButtonByName(name);
	}
	
	return NULL;
	
}

static void MainPageRun(void *pParams)
{
	int err;
	InputData tData;
	PButton ptButton;
	PDispBuff ptDispBuff = GetDispBuff();
	
	/* read config file */
	err = ParseConfigFile();
	if(err){
		return ;
	}
	/* generate ui and button */
	GenerateButton();
	while(1)
	{
		/* read input */
		err = GetInputData(&tData);
		if (err)
			continue;
		/* get button */
		ptButton = GetButtonByInputEvent(&tData);
		if (!ptButton)
			continue;

		/* handle */
		
		ptButton->OnPressed(ptButton, ptDispBuff, &tData);
	}
}

static PageAction g_tMainPage = {
	.name = "main",
	.Run  = MainPageRun,
};


void MainPageRegister(void)
{
	PageRegister(&g_tMainPage);
}


