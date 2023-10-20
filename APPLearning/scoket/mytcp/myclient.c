#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

/* socket
 * connect
 * send/recv
 */
#define SERVER_PORT 8888

 int main(int argc, char **argv){
 	
	int iSocketClient;
	int iAddrLen;
	int iSendLen;
	int ierr;
	unsigned char ucSendBuf[1000];
	struct sockaddr_in tSocketServerAddr;
	
	if (argc != 2)
	{
		printf("Usage:\n");
		printf("%s <server_ip>\n", argv[0]);
		return -1;
	}
	
	iSocketClient = socket(AF_INET, SOCK_STREAM, 0);	
 	if(iSocketClient == -1){
		 printf("socket err!");
		 return -1;
	}
	
	tSocketServerAddr.sin_family = AF_INET;
	tSocketServerAddr.sin_port = htons(SERVER_PORT);
//	tSocketClientAddr.sin_addr.s_addr = INADDR_ANY;
	if (inet_aton(argv[1], &tSocketServerAddr.sin_addr) == 0) {
		printf("Invalid address\n");
		return -1;
	}
	memset(tSocketServerAddr.sin_zero, 0, 8);

	iAddrLen = sizeof(struct sockaddr);
	ierr = connect(iSocketClient, (const struct sockaddr *) &tSocketServerAddr,
                   iAddrLen);
	if(ierr == -1){
		printf("connect err!");
		return -1;
	}	
	while(1){
	
		if(fgets(ucSendBuf, 999, stdin)){
			iSendLen = send(iSocketClient, ucSendBuf, strlen(ucSendBuf), 0);
			if (iSendLen <= 0)
			{
				close(iSocketClient);
				return -1;
			}
		}
	}
	return 0;
 }




