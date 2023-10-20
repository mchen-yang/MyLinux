#include <stdio.h>
#include <netinet/in.h>   //for souockaddr_in
#include <sys/types.h>     
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

/* socket
 * bind
 * listen
 * accept
 * send/recv
 */
#define SERVER_PORT 8888

#define BACKLOG			10
int main(int argc, char **argv){
	int iSocketServer;
	int iSocketClient;
	struct sockaddr_in tSocketServerAddr;
	int iAddrLen;
	int iRecvLen;
	int ierr;
	int iret;
	unsigned char ucRecvBuf[1000];

	int iClientNum = -1;

	signal(SIGCHLD,SIG_IGN);
		
	iSocketServer = socket(AF_INET, SOCK_STREAM, 0);//TODO,why is the protocol 0?
	if(iSocketServer == -1){
		printf("socket err!");
		return -1;
	}

	tSocketServerAddr.sin_family = AF_INET;
	tSocketServerAddr.sin_port = htons(SERVER_PORT);
	tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	memset(tSocketServerAddr.sin_zero, 0, 8);
//	iAddrLen = 999;
	iAddrLen = sizeof(struct sockaddr);

	ierr = bind(iSocketServer, (const struct sockaddr *) &tSocketServerAddr,
									iAddrLen);
	if(ierr == -1){
		printf("bind err!");
		return -1;
	}

	iret = listen(iSocketServer, BACKLOG);
	
	if(iret == -1){
		printf("listen err!");
		return -1;
	}
	while(1){
		
		iSocketClient = accept(iSocketServer, (struct sockaddr *) &tSocketServerAddr, &iAddrLen);
		

		
		if(iSocketClient != -1){
			iClientNum++;
			printf("Get connect from client %d : %s\n",iClientNum, inet_ntoa(tSocketServerAddr.sin_addr));

			if(!fork()){

				while(1){
					iRecvLen = recv(iSocketClient, ucRecvBuf, 999, 0);
					if(iRecvLen <= 0){
						printf("recv is err!");
						close(iSocketClient);
						return -1;
					}
					else{
						ucRecvBuf[iRecvLen] = '\0';
						printf("Get Msg From Client %d: %s\n", iClientNum, ucRecvBuf);
					}
				}
				
			}
			else{

			}
			
		}
	}
	
	return 0;
}


