#include<stdio.h>
#include<WinSock2.h>
#pragma comment(lib,"wsock32.lib")
typedef struct sockaddr_in sockaddr_in;
int main()
{
	WSADATA wsdata;
	if(WSAStartup(MAKEWORD(2,2),&wsdata) != 0){
		return 0;
	}
	SOCKET connect_socket = socket(AF_INET,SOCK_STREAM,0);
	sockaddr_in sockad;
	sockad.sin_family = AF_INET;
	sockad.sin_port = htons(6000);
	sockad.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	connect(connect_socket,(SOCKADDR *)&sockad,sizeof(SOCKADDR));
	char sendbuff[1000] = {0};
	char recivebuff[1000] = {0};
	while(1){
		printf("input: ");
		gets(sendbuff);
		send(connect_socket,sendbuff,sizeof(sendbuff)+1,0);
		recv(connect_socket,recivebuff,sizeof(recivebuff),0);
		printf("%s\n",recivebuff);
	}
	closesocket(connect_socket);
	WSACleanup();
	return 0;
}
