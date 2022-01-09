#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#pragma comment(lib, "libws2_32.lib");

int main()
{
	WSADATA wsdata;
	if(WSAStartup(MAKEWORD(2,2),&wsdata) != 0){
		return 0;
	}
	/*�������������׽���--IPv4Э�飬��������ͨ�ţ�TCPЭ��*/
	SOCKET server_socket = socket(AF_INET,SOCK_STREAM,0);
	if(server_socket==INVALID_SOCKET){
		printf("socket failed with error:%d\n",WSAGetLastError());
		return 1;
	}

	SOCKADDR_IN addrsrv;		
	addrsrv.sin_family = AF_INET;//����ΪIPͨ��
	addrsrv.sin_port = htons(8000);//�������˿ں�				
	addrsrv.sin_addr.s_addr = INADDR_ANY;//������IP��ַ--�������ӵ����б��ص�ַ��		
	/*���׽��ְ󶨵��������������ַ��*/
	if(bind(server_socket, (SOCKADDR *)&addrsrv, sizeof(SOCKADDR))!=0){
		printf("bind failed with error:%d\n",WSAGetLastError());	
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}
	/*������������--�������г���Ϊ5*/
	if(listen(server_socket,5)!=0){
		printf("listen failed with error:%d\n",WSAGetLastError());	
		closesocket(server_socket);
		WSACleanup();
		return 1;		
	}
	SOCKADDR_IN buffptr;						
	int bufflen=sizeof(SOCKADDR);
	/*�ȴ��ͻ����������󵽴�*/
	SOCKET accept_socket = accept(server_socket,(SOCKADDR*)&buffptr,&bufflen);
	if(accept_socket==INVALID_SOCKET){
		printf("accept failed with error:%d\n",WSAGetLastError());	
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}						
	char recivebuff[1000] = {0};
	int len;
	/*���տͻ��˵����ݲ����䷢�͸��ͻ���--recv���ؽ��յ����ֽ�����send���ط��͵��ֽ���*/
	while(len=recv(accept_socket,recivebuff,sizeof(recivebuff),0)>0){
		if(send(accept_socket,recivebuff,strlen(recivebuff)+1,0)<=0){
			printf("send failed with error:%d\n",WSAGetLastError());	
			closesocket(accept_socket);
			WSACleanup();
			return 1;
		}
		printf("%s\n",recivebuff);
	}
	printf("i have closed!\n");
	closesocket(accept_socket);
	closesocket(server_socket);
	return 0;
}


