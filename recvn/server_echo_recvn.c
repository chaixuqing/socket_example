#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <windows.h> 
#define N  10
#pragma comment(lib, "libws2_32.lib");


int recvn(SOCKET s, char * recvbuf, unsigned int fixedlen);

int main()
{
	WSADATA wsdata;
	if(WSAStartup(MAKEWORD(2,2),&wsdata) != 0){
		return 0;
	}
	SOCKET server_socket = socket(AF_INET,SOCK_STREAM,0);    //创建一个套接字
//	SOCKET client_socket = socket(AF_INET,SOCK_STREAM,0); 
	SOCKADDR_IN addrsrv;			//bind函数要绑定信息，需要用到一个结构体，里面存放本地地址的信息，他根据使用的TCP协议而变为SOCKADDR_IN类型
	addrsrv.sin_family = AF_INET;
	addrsrv.sin_port = htons(8000);				//指定要分配给这个套接字的端口
	addrsrv.sin_addr.s_addr = INADDR_ANY;		//指定本机的ip地址
	if(bind(server_socket, (SOCKADDR *)&addrsrv, sizeof(SOCKADDR))!=0){
		printf("bind failed with eoor:%d\n",WSAGetLastError());	
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}
	if(listen(server_socket,10)!=0){
		printf("listen failed with eoor:%d\n",WSAGetLastError());	
		closesocket(server_socket);
		WSACleanup();
		return 1;		
	}
	SOCKADDR_IN buffptr;						//创建一个结构体，用于接收客户端发来请求的时候，携带的客户端的地址信息，IP和端口号等
	int bufflen=sizeof(SOCKADDR);
	SOCKET accept_socket = accept(server_socket,(SOCKADDR*)&buffptr,&bufflen);
	if(accept_socket==INVALID_SOCKET){
		printf("accept failed with eoor:%d\n",WSAGetLastError());	
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}						
	char recivebuff[1000] = {0};
	int len;
	while(len=recvn(accept_socket,recivebuff,N)>0){
		if(send(accept_socket,recivebuff,strlen(recivebuff),0)<=0){
			printf("send failed with error:%d\n",WSAGetLastError());	
			closesocket(accept_socket);
			WSACleanup();
			return 1;
		}
		printf("%s\n",recivebuff);
		memset(recivebuff,0,1000);
	}
	printf("i have closed!\n");
	closesocket(accept_socket);
	closesocket(server_socket);
	
	Sleep(10000);
	return 0;
}


int recvn(SOCKET s, char * recvbuf, unsigned int fixedlen)
{
	int iResult;    //�洢����recv�����ķ���ֵ
	int cnt;         //����ͳ������ڹ̶����ȣ�ʣ������ֽ���δ����
	cnt = fixedlen;
	while ( cnt > 0 ) 
	{
		iResult = recv(s, recvbuf, cnt, 0);
		if ( iResult < 0 ){
			//���ݽ��ճ��ִ��󣬷���ʧ��
			printf("���շ�������: %d\n", WSAGetLastError());
			return -1;
		}
		if ( iResult == 0 ){
			//�Է��ر����ӣ������ѽ��յ���С��fixedlen���ֽ���
			printf("���ӹر�\n");
			return fixedlen - cnt;
		}
		//printf("���յ����ֽ���: %d\n", iResult);
		//���ջ���ָ������ƶ�
		recvbuf +=iResult;
		//����cntֵ
		cnt -=iResult;         
	}
	return fixedlen;
}


