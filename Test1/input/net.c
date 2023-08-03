#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

/* My include */
#include <input_manager.h>

#define SERVER_PORT 8888

static int g_iSocketServer;

static int NetDeviceInit(void) {

	struct sockaddr_in tSocketServerAddr;
	int iRet;

	g_iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == g_iSocketServer)
	{
		printf("socket error!\n");
		return -1;
	}

	
	tSocketServerAddr.sin_family      = AF_INET;
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	memset(tSocketServerAddr.sin_zero, 0, 8);

	iRet = bind(g_iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (-1 == iRet)
	{
		printf("bind error!\n");
		return -1;
	}

	return 0;
}

static int NetDeviceExit(void) {

	close(g_iSocketServer);	

	return 0;
}

static int NetGetInput(PInputData ptInputData) {
	unsigned int iAddrLen;
	int iRecvLen;
	char acRecvBuf[1000];
	struct sockaddr_in tSocketClientAddr;
	
	iAddrLen = sizeof(struct sockaddr);
	iRecvLen = recvfrom(g_iSocketServer, acRecvBuf, 999, 0, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
	if (iRecvLen > 0)
	{
		acRecvBuf[iRecvLen] = '\0';
		
		ptInputData->InputType = INPUT_TYPE_NET;
		gettimeofday(&ptInputData->tTime, NULL);
		strncpy(ptInputData->InputStr, acRecvBuf, 1000);
		ptInputData->InputStr[999] = '\0';
		return 0;
	}
	else{
		return -1;
	}
}


static InputOper g_tNetScreenOper = {
	.name			= "netscreen",
	.InputInit		= NetDeviceInit,
	.InputExit		= NetDeviceExit,
	.GetInput		= NetGetInput,
};

void NetInputRegister(void) {
	RegisterInput(&g_tNetScreenOper);
}
#if 0

int main(int argc, char **argv){
	InputData tInputData;

	int ret;
	g_tNetScreenOper.InputInit();

	while(1){
		ret = g_tNetScreenOper.GetInput(&tInputData);
		if(ret == -1){
			printf("GetInput err!\n");
			return -1;
		}
		else{
			printf("Type			= %d\n", tInputData.InputType);
			printf("neirong		= %s\n", tInputData.InputStr);
		}
	}
	return 0;
}
#endif
