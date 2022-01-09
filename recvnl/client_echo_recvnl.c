#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#define MAXLEN 1000 
#pragma comment(lib, "libws2_32.lib");

int recvvl(SOCKET s, char * recvbuf, unsigned int recvbuflen); 
int recvn(SOCKET s, char * recvbuf, unsigned int fixedlen);

int main()
{
	WSADATA wsdata;
	if(WSAStartup(MAKEWORD(2,2),&wsdata) != 0){
		printf("WSAStratup failed with error:%d\n",WSAGetLastError());
		return 1;
	}
	/*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
	SOCKET connect_socket = socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN sockad;//服务器端网络地址结构体
	sockad.sin_family = AF_INET;//设置为IP通信
	sockad.sin_port = htons(8000);//服务器端口号
	sockad.sin_addr.s_addr = inet_addr("127.0.0.1");//服务器IP地址
	/*将套接字绑定到服务器的网络地址上*/
	if(connect(connect_socket,(SOCKADDR*)&sockad,sizeof(SOCKADDR))!=0){
		printf("unable to connect to server!\n");
		return 1;
	}
	char sendbuff[1000] = {0};
	char recvbuff[1000] = {0}; 
	/*循环的发送接收信息并打印接收信息--recv返回接收到的字节数，send返回发送的字节数*/
	while(TRUE){
		printf("input: ");
		gets(sendbuff);
		if(!strcmp(sendbuff,"q"))
			break;
		unsigned int len = (unsigned int)strlen(sendbuff);
		len = htonl(len);
		//发送消息头 
		if(send(connect_socket, (char*)&len, sizeof(unsigned int), 0) <= 0) 
		{
			printf("send failed !code:%d\n",WSAGetLastError());
			closesocket(connect_socket);
			WSACleanup();
			return 1;
		}
		//发送消息体 
		if(send(connect_socket,sendbuff,strlen(sendbuff),0) <= 0) {
			printf("send failed with error:%d\n",WSAGetLastError());
			closesocket(connect_socket);
			WSACleanup();
			return 1;
	    }
		if(recvvl(connect_socket,recvbuff,1000)<=0){
			closesocket(connect_socket);
			WSACleanup();
			return 1;
		}
		printf("%s\n",recvbuff);
		memset(recvbuff,0,1000);
	}
	printf("i have closed!\n");
	closesocket(connect_socket);//关闭套接字
	WSACleanup();
	return 0;
}

int recvn(SOCKET s, char * recvbuf, unsigned int fixedlen)
{
	int iResult;    //存储单次recv操作的返回值
	int cnt;         //用于统计相对于固定长度，剩余多少字节尚未接收
	cnt = fixedlen;
	while ( cnt > 0 ) 
	{
		iResult = recv(s, recvbuf, cnt, 0);
		if ( iResult < 0 ){
			//数据接收出现错误，返回失败
			printf("接收发生错误: %d\n", WSAGetLastError());
			return -1;
		}
		if ( iResult == 0 ){
			//对方关闭连接，返回已接收到的小于fixedlen的字节数
			printf("连接关闭\n");
			return fixedlen - cnt;
		}
		//printf("接收到的字节数: %d\n", iResult);
		//接收缓存指针向后移动
		recvbuf +=iResult;
		//更新cnt值
		cnt -=iResult;         
	}
	return fixedlen;
}


int recvvl(SOCKET s, char * recvbuf, unsigned int recvbuflen)
{
	int iResult;//存储单次recvn操作的返回值
	unsigned int reclen; //用于存储报文头部存储的长度信息
	//获取接收报文长度信息
	iResult = recvn(s, ( char * )&reclen, sizeof( unsigned int ));
	//printf("reclen:%d\n",reclen);
	if ( iResult !=sizeof ( unsigned int )) {
		//如果长度字段在接收时没有返回一个整型数据就返回（连接关闭）或-1（发生错误）
		if ( iResult == -1 ) {
			printf("接收发生错误: %d\n", WSAGetLastError());
			return -1;
		}
		else {
			printf("连接关闭\n");
			return 0;
		}
	}
	//转换网络字节顺序到主机字节顺序
	reclen = ntohl( reclen );
 
	if ( reclen > recvbuflen ) {
		//如果recvbuf没有足够的空间存储变长消息，则接收该消息并丢弃，返回错误
		while ( reclen > 0){
			iResult = recvn( s, recvbuf, recvbuflen );
			if ( iResult != recvbuflen ) {
				//如果变长消息在接收时没有返回足够的数据就返回（连接关闭）或-1（发生错误）
				if ( iResult == -1 ) {
					printf("接收发生错误: %d\n", WSAGetLastError());
					return -1;
				}
				else {
					printf("连接关闭\n");
					return 0;
				}
			}
			reclen -= recvbuflen;
			//处理最后一段数据长度
			if ( reclen < recvbuflen )
				recvbuflen = reclen;
		}
		printf("可变长度的消息超出预分配的接收缓存\r\n");
		return -1;
	}
	//接收可变长消息
	iResult = recvn( s, recvbuf, reclen );
	if ( iResult != reclen ){
		//如果消息在接收时没有返回足够的数据就返回（连接关闭）或-1（发生错误）
		if ( iResult == -1 ) {
			printf("接收发生错误: %d\n", WSAGetLastError());
			return -1;
		}
		else {
			printf("连接关闭\n");
			return 0;
		}
	}
	return iResult;
}
