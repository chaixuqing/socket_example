#include<stdio.h>
#include<WinSock2.h>
#include <time.h>
#include <wtypes.h>
#pragma comment(lib, "libws2_32.lib");
int main()
{
	WSADATA wsdata;
	if(WSAStartup(MAKEWORD(2,2),&wsdata) != 0){
		return -1;
	}
	SOCKET server_socket = socket(AF_INET,SOCK_STREAM,0);    //创建一个套接字
	SOCKADDR_IN addrsrv;			//bind函数要绑定信息，需要用到一个结构体，里面存放本地地址的信息，他根据使用的TCP协议而变为SOCKADDR_IN类型
	addrsrv.sin_family = AF_INET;
	addrsrv.sin_port = htons(6000);				//指定要分配给这个套接字的端口
	addrsrv.sin_addr.s_addr = INADDR_ANY;		//指定本机的ip地址
	bind(server_socket, (SOCKADDR *)&addrsrv, sizeof(SOCKADDR));
	listen(server_socket,10);					//监听此端口
	SOCKADDR_IN buffptr;						//创建一个结构体，用于接收客户端发来请求的时候，携带的客户端的地址信息，IP和端口号等
	int bufflen = sizeof(SOCKADDR);
	while(TRUE)
	{
		SOCKET accept_socket = accept(server_socket,(SOCKADDR*)&buffptr,&bufflen);		//开始不断的接受客户端请求的到来
		char sendbuff[100] = {0};														//发送缓冲区
		SYSTEMTIME stCurTime = {0};
		GetLocalTime(&stCurTime);
		sprintf(sendbuff,"%d/%d/%d\t%d:%d:%d\n",stCurTime.wYear,stCurTime.wMonth,stCurTime.wDay,stCurTime.wHour,stCurTime.wMinute,stCurTime.wSecond);					//把我需要的信息写入到发送缓冲区，利用inet_ntoa函数将buffptr指向的存储客户端地址信息结构体中的sin_addr转换为点分十进制的字符串
		send(accept_socket,sendbuff,sizeof(sendbuff),0);					//之所以+1是因为发送一个字符数组过去之后一定要在最后留有一个\0字符的位置
		closesocket(accept_socket);					//本次处理请求结束，释放为此次请求所分配的资源，关闭连接套接字。
	}
	return 0;
}
