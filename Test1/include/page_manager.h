#ifndef _PAGE_MANAGER_H_
#define _PAGE_MANAGER_H_

#include <common.h>


typedef struct PageAction {
	char *name;
	void (*Run)(void *pParams);
	struct PageAction *ptNext;
}PageAction, *PPageAction;


void PageRegister(PPageAction ptPageAction);
void PageSystemRegister(void);
PPageAction Page(char *name);

#endif // !_PAGE_MANAGER_H_