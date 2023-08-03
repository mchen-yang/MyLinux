#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <sys/time.h>

#ifndef NULL
#define NULL (void *)0
#endif

#define INPUT_TYPE_TOUCH 1
#define INPUT_TYPE_NET   2



typedef struct InputData{
	struct timeval tTime; 
	int InputType;
	int X;
	int Y;
	int Pressure;
	char InputStr[1000];
}InputData, *PInputData;

typedef struct InputOper{
	char *name;
	int (*InputInit)(void);
	int (*InputExit)(void);
	int (*GetInput)(PInputData ptInputData);
	struct InputOper* pNext;
}InputOper, *PInputOper;

void RegisterInput(PInputOper ptInputOper);
void InputSystemRegister(void);
int SelectDefaultInput(char* name);
int InitInputDevice(void); 
int GetInputData(PInputData ptData);



#endif




