#ifndef _UI_H_
#define _UI_H_

#include <common.h>
#include <font_manager.h>
#include <input_manager.h>
#include <disp_manager.h>

#define BUTTON_DEFAULT_COLOR 0xFF0000
#define BUTTON_PRESSED_COLOR 0x00FF00
#define BUTTON_PERCENT_COLOR 0x0000ff

#define BUTTON_TEXT_COLOR    0x000000

struct Button;//declaration Button

typedef int (*ONDRAW_FUNC)(struct Button *ptButton, PDispBuff ptDispBuff);
typedef int (*ONPRESSED_FUNC)(struct Button *ptButton, PDispBuff ptDispBuff, PInputData ptInputData);

typedef struct Button {
	char* name;
	int FontSize;
	int status;
	Region tRegion;
	ONDRAW_FUNC OnDraw;
	ONPRESSED_FUNC OnPressed;
	struct Button* ptNext;
}Button, *PButton;


void InitButton(PButton ptButton, char *name, PRegion ptRegion, ONDRAW_FUNC OnDraw, ONPRESSED_FUNC OnPressed);

#endif // !_UI_H_
