
#include <font_manager.h>

PFontOpr g_ptFonts = NULL;
static PFontOpr g_ptDefaulFontOpr = NULL;

void FontsRegister(PFontOpr ptFontOper) {
	ptFontOper->ptNext = g_ptFonts;
	g_ptFonts = ptFontOper;
}

void FontInit(void) {
	/* register FreeType */
	extern void FreeTypeInit();
	FreeTypeInit();

}

int SelectAndInitFont(char *aFontOprName, char *aFontFileName)
{
	PFontOpr ptTmp = g_ptFonts;
	while (ptTmp)
	{
		if (strcmp(ptTmp->name, aFontOprName) == 0)
			break;
		ptTmp = ptTmp->ptNext;
	}

	if (!ptTmp)
		return -1;

	g_ptDefaulFontOpr = ptTmp;
	return ptTmp->FontInit(aFontFileName);
}

int SetFontSize(int iFontSize)
{
	return g_ptDefaulFontOpr->SetFontSize(iFontSize);
}

int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap)
{
	return g_ptDefaulFontOpr->GetFontBitMap(dwCode, ptFontBitMap);
}

//int InitInputDevice(void) {
//	int ret;
//	pthread_t tid;
//	/*for each inputdevice, init, creat task */
//	PInputOper pTemp = g_InputDevs;
//	while (pTemp) {
//	
//		/* init */
//		ret = pTemp->InputInit();
//		if(!ret){
//		
//			/* create task */
//			ret = pthread_create(&tid, NULL, input_recv_thread_func, pTemp);//TODO:我不懂Linux线程的创建
//		}
//		pTemp = pTemp->pNext;
//	}
//	return 0;
//}



