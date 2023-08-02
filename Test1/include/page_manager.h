#ifndef _PAGE_MANAGER_H_
#define _PAGE_MANAGER_H_

#include <common.h>
#include <font_manager.h>
#include <input_manager.h>
#include <disp_manager.h>


typedef struct PageAction {
	char *name;
	void (*Run)(void *pParams);
	struct PageAction *ptNext;
}PageAction, *PPageAction;


void PageRegister(PPageAction ptPageAction);
void PagesRegister(void);
PPageAction Page(char *name);

#endif // !_PAGE_MANAGER_H_