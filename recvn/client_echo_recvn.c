#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#define MAXLEN 1000 
#pragma comment(lib, "libws2_32.lib");


static char recivebuff[1000] = {0};//数据传送的缓冲区,数据初始化--清零
static int read_cnt=0;
static char* read_ptr;
int recvline(SOCKET accept_socket,char *recvbuff);
int my_read(SOCKET accept_socket, char *p);

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
	char recivebuff[1000] = {0}; 
	/*循环的发送接收信息并打印接收信息--recv返回接收到的字节数，send返回发送的字节数*/
	while(TRUE){
		printf("input: ");
		gets(sendbuff);
		if(!strcmp(sendbuff,"q"))
			break;
		if(send(connect_socket,sendbuff,strlen(sendbuff),0) <= 0) {
			printf("send failed with error:%d\n",WSAGetLastError());
			closesocket(connect_socket);
			WSACleanup();
			return 1;
	    }
		if(recvline(connect_socket,recivebuff)<=0){
			closesocket(connect_socket);
			WSACleanup();
			return 1;
		}
		printf("%s\n",recivebuff);
	}
	printf("i have closed!\n");
	closesocket(connect_socket);//关闭套接字
	WSACleanup();
	return 0;
}

int recvline(SOCKET accept_socket,char *recvbuff){
	int n,rc;
	char c,*ptr;
	ptr=recvbuff;
	for(n=1;n<MAXLEN;n++){
		if((rc = my_read(accept_socket, &c)) == 1) {
			*ptr++ = c;
			if (c == '0')
				break;	/* newline is stored, like fgets() */
		} else if (rc == 0) {
			*ptr = 0;
			return(n - 1);	/* EOF, n - 1 bytes were read */
		} else
			return(-1);		/* error, errno set by read() */
	}
	*ptr = 0;	/* null terminate like fgets() */
	return(n);
}

int my_read(SOCKET accept_socket, char *p){
	if (read_cnt <= 0) {
		if((read_cnt = recv(accept_socket,recivebuff, sizeof(recivebuff),0)) < 0)
			return(-1);
		else if (read_cnt == 0)
			return(0);
		read_ptr = recivebuff;
	}

	read_cnt--;
	*p = *read_ptr++;
	return(1);
}

