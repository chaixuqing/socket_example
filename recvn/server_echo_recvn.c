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
	SOCKET server_socket = socket(AF_INET,SOCK_STREAM,0);    //åˆ›å»ºä¸€ä¸ªå¥—æŽ¥å­—
//	SOCKET client_socket = socket(AF_INET,SOCK_STREAM,0); 
	SOCKADDR_IN addrsrv;			//bindå‡½æ•°è¦ç»‘å®šä¿¡æ¯ï¼Œéœ€è¦ç”¨åˆ°ä¸€ä¸ªç»“æž„ä½“ï¼Œé‡Œé¢å­˜æ”¾æœ¬åœ°åœ°å€çš„ä¿¡æ¯ï¼Œä»–æ ¹æ®ä½¿ç”¨çš„TCPåè®®è€Œå˜ä¸ºSOCKADDR_INç±»åž‹
	addrsrv.sin_family = AF_INET;
	addrsrv.sin_port = htons(8000);				//æŒ‡å®šè¦åˆ†é…ç»™è¿™ä¸ªå¥—æŽ¥å­—çš„ç«¯å£
	addrsrv.sin_addr.s_addr = INADDR_ANY;		//æŒ‡å®šæœ¬æœºçš„ipåœ°å€
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
	SOCKADDR_IN buffptr;						//åˆ›å»ºä¸€ä¸ªç»“æž„ä½“ï¼Œç”¨äºŽæŽ¥æ”¶å®¢æˆ·ç«¯å‘æ¥è¯·æ±‚çš„æ—¶å€™ï¼Œæºå¸¦çš„å®¢æˆ·ç«¯çš„åœ°å€ä¿¡æ¯ï¼ŒIPå’Œç«¯å£å·ç­‰
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
	int iResult;    //´æ´¢µ¥´Îrecv²Ù×÷µÄ·µ»ØÖµ
	int cnt;         //ÓÃÓÚÍ³¼ÆÏà¶ÔÓÚ¹Ì¶¨³¤¶È£¬Ê£Óà¶àÉÙ×Ö½ÚÉÐÎ´½ÓÊÕ
	cnt = fixedlen;
	while ( cnt > 0 ) 
	{
		iResult = recv(s, recvbuf, cnt, 0);
		if ( iResult < 0 ){
			//Êý¾Ý½ÓÊÕ³öÏÖ´íÎó£¬·µ»ØÊ§°Ü
			printf("½ÓÊÕ·¢Éú´íÎó: %d\n", WSAGetLastError());
			return -1;
		}
		if ( iResult == 0 ){
			//¶Ô·½¹Ø±ÕÁ¬½Ó£¬·µ»ØÒÑ½ÓÊÕµ½µÄÐ¡ÓÚfixedlenµÄ×Ö½ÚÊý
			printf("Á¬½Ó¹Ø±Õ\n");
			return fixedlen - cnt;
		}
		//printf("½ÓÊÕµ½µÄ×Ö½ÚÊý: %d\n", iResult);
		//½ÓÊÕ»º´æÖ¸ÕëÏòºóÒÆ¶¯
		recvbuf +=iResult;
		//¸üÐÂcntÖµ
		cnt -=iResult;         
	}
	return fixedlen;
}


