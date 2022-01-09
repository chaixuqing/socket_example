#include<stdio.h>
#include<WinSock2.h>

int main()
{
	WSADATA wsdata;
	if(WSAStartup(MAKEWORD(2,2),&wsdata) != 0){
		return 0;
	}
	SOCKET server_socket = socket(AF_INET,SOCK_STREAM,0);   
	SOCKADDR_IN addrsrv;			
	addrsrv.sin_family = AF_INET;
	addrsrv.sin_port = htons(6000);				
	addrsrv.sin_addr.S_un.S_addr = INADDR_ANY;		
	bind(server_socket, (SOCKADDR *)&addrsrv, sizeof(SOCKADDR));
	listen(server_socket,10);					
	SOCKADDR_IN buffptr;						
	int bufflen = sizeof(SOCKADDR);
	while(1){
		SOCKET accept_socket = accept(server_socket,(SOCKADDR*)&buffptr,&bufflen);		
		char sendbuff[1000] = {0};														
		char recivebuff[1000] = {0};												
		sprintf(sendbuff,"welcome %s to www.fengzixu.net",inet_ntoa(buffptr.sin_addr));					
		send(accept_socket,sendbuff,sizeof(sendbuff)+1,0);					
		recv(accept_socket,recivebuff,sizeof(recivebuff),0);
		printf("%s\n",recivebuff);
		closesocket(accept_socket);				
	}
	return 0;
}
