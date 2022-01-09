#include<stdio.h>
#include<WinSock2.h>
#pragma comment(lib, "libws2_32.lib");

int main()
{
	WSADATA wsdata;
	if(WSAStartup(MAKEWORD(2,2),&wsdata) != 0){
		return -1;
	}
	SOCKET connect_socket = socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN sockad;
	sockad.sin_family = AF_INET;
	sockad.sin_port = htons(6000);
	sockad.sin_addr.s_addr = inet_addr("127.0.0.1");
	connect(connect_socket,(SOCKADDR*)&sockad,sizeof(SOCKADDR));
	char recivebuff[100] = {0};
    int i;
	while((i=recv(connect_socket,recivebuff,sizeof(recivebuff),0))>0){
		printf("recv bytes=%d\n",i); 
        printf("%s\n",recivebuff);
    }
	
	closesocket(connect_socket);
	WSACleanup();
	return 0;
}
