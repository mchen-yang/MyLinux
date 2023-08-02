#include <common.h>

#include <string.h>
#include <page_manager.h>


static PPageAction g_ptPages;

void PageRegister(PPageAction ptPageAction)
{
	ptPageAction->ptNext = g_ptPages;
	g_ptPages = ptPageAction;
}

PPageAction Page(char *name)
{
	PPageAction ptTmp = g_ptPages;

	while (ptTmp)
	{
		if (strcmp(name, ptTmp->name) == 0)
			return ptTmp;
		ptTmp = ptTmp->ptNext;
	}

	return NULL;
}

void PagesRegister(void)
{
	extern void MainPageRegister(void);
	MainPageRegister();
}


