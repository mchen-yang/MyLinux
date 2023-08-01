
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>

#include <input_manager.h>
//TODO:zenmegao
static pthread_mutex_t g_tMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  g_tConVar = PTHREAD_COND_INITIALIZER;

/* ����ײ��LCD, WEB�� */
static PInputOper g_InputDevs = NULL;
static PInputOper g_InputDefault = NULL;

//static function declaration
static int isInputBufferFull(void);
static int isInputBufferEmpty(void);
static void PutInputDataToBuffer(PInputData ptdata);
static int GetInputDataFromBuffer(PInputData ptdata);
static void *input_recv_thread_func (void *data);

/* start of ʵ�ֻ���buffer */
#define BUFFER_LEN 20
static int g_iRead  = 0;
static int g_iWrite = 0;
static InputData g_atInputbuffer[BUFFER_LEN];

static int isInputBufferFull(void)
{
	return (g_iRead == ((g_iWrite + 1) % BUFFER_LEN));
}

static int isInputBufferEmpty(void)
{
	//TODO:Ϊ������
	return (g_iRead == g_iWrite);
}

static void PutInputDataToBuffer(PInputData ptdata){
	/*put when not full */
	if(!isInputBufferFull()){
		g_atInputbuffer[g_iWrite] = *ptdata;
		g_iWrite = (g_iWrite + 1) % BUFFER_LEN;
	}

}
static int GetInputDataFromBuffer(PInputData ptdata){
	/*get when not empty*/
	if(!isInputBufferEmpty()){
		*ptdata = g_atInputbuffer[g_iRead] ;
		g_iRead = (1 + g_iRead) % BUFFER_LEN;
		return 1;
	}
	return 0;
}

/* end of ʵ�ֻ���buffer */

void RegisterInput(PInputOper ptInputOper) {
	ptInputOper->pNext = g_InputDevs;
	g_InputDevs = ptInputOper;
}

void InputInit(void) {
	/* register touchscreen */
	extern void TouchScreenInit();
	TouchScreenInit();
	
	 /* register net*/
	extern void NetScreenInit();
	NetScreenInit();
	//web
}

int SelectDefaultInput(char* name) {
	PInputOper pTemp = g_InputDevs;
	while (pTemp) {
		if (strcmp(pTemp->name, name) == 0) {
			g_InputDefault = pTemp;
			return 0;
		}
		pTemp = pTemp->pNext;
	}
	return -1;
}

static void *input_recv_thread_func (void *data)
{
	PInputOper InputDev = (PInputOper)data;
	InputData	tData;
	int ret;
	while (1)
	{
		ret = InputDev->GetInput(&tData);
		if(!ret){
			/* �������� */
			pthread_mutex_lock(&g_tMutex);
			PutInputDataToBuffer(&tData);			
			
			/* ���ѵȴ����ݵ��߳� */		
			pthread_cond_signal(&g_tConVar);	//TODO:������֪ͨ�����߳� 
			pthread_mutex_unlock(&g_tMutex);
		}
	}

	return NULL;
}


int InitInputDevice(void) {
	int ret;
	pthread_t tid;
	/*for each inputdevice, init, creat task */
	PInputOper pTemp = g_InputDevs;
	while (pTemp) {
	
		/* init */
		ret = pTemp->InputInit();
		if(!ret){
		
			/* create task */
			ret = pthread_create(&tid, NULL, input_recv_thread_func, pTemp);//TODO:�Ҳ���Linux�̵߳Ĵ���
		}
		pTemp = pTemp->pNext;
	}
	return 0;
}

int GetInputData(PInputData ptData){

	int ret;
	/* get */
	pthread_mutex_lock(&g_tMutex);
	if(GetInputDataFromBuffer(ptData)){
		pthread_mutex_unlock(&g_tMutex);
		return 0;
	}
	else{
		/* wait */
		pthread_cond_wait(&g_tConVar, &g_tMutex); 
		//TODO:�Ƿ�����Ż�
		if (GetInputDataFromBuffer(ptData))
		{
			ret = 0;
		}
		else
		{
			ret = -1;
		}
		pthread_mutex_unlock(&g_tMutex);
	}

	return ret;
}



