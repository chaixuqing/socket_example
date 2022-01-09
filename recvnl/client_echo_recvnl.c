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
	/*�����ͻ����׽���--IPv4Э�飬��������ͨ�ţ�TCPЭ��*/
	SOCKET connect_socket = socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN sockad;//�������������ַ�ṹ��
	sockad.sin_family = AF_INET;//����ΪIPͨ��
	sockad.sin_port = htons(8000);//�������˿ں�
	sockad.sin_addr.s_addr = inet_addr("127.0.0.1");//������IP��ַ
	/*���׽��ְ󶨵��������������ַ��*/
	if(connect(connect_socket,(SOCKADDR*)&sockad,sizeof(SOCKADDR))!=0){
		printf("unable to connect to server!\n");
		return 1;
	}
	char sendbuff[1000] = {0};
	char recvbuff[1000] = {0}; 
	/*ѭ���ķ��ͽ�����Ϣ����ӡ������Ϣ--recv���ؽ��յ����ֽ�����send���ط��͵��ֽ���*/
	while(TRUE){
		printf("input: ");
		gets(sendbuff);
		if(!strcmp(sendbuff,"q"))
			break;
		unsigned int len = (unsigned int)strlen(sendbuff);
		len = htonl(len);
		//������Ϣͷ 
		if(send(connect_socket, (char*)&len, sizeof(unsigned int), 0) <= 0) 
		{
			printf("send failed !code:%d\n",WSAGetLastError());
			closesocket(connect_socket);
			WSACleanup();
			return 1;
		}
		//������Ϣ�� 
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
	closesocket(connect_socket);//�ر��׽���
	WSACleanup();
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


int recvvl(SOCKET s, char * recvbuf, unsigned int recvbuflen)
{
	int iResult;//�洢����recvn�����ķ���ֵ
	unsigned int reclen; //���ڴ洢����ͷ���洢�ĳ�����Ϣ
	//��ȡ���ձ��ĳ�����Ϣ
	iResult = recvn(s, ( char * )&reclen, sizeof( unsigned int ));
	//printf("reclen:%d\n",reclen);
	if ( iResult !=sizeof ( unsigned int )) {
		//��������ֶ��ڽ���ʱû�з���һ���������ݾͷ��أ����ӹرգ���-1����������
		if ( iResult == -1 ) {
			printf("���շ�������: %d\n", WSAGetLastError());
			return -1;
		}
		else {
			printf("���ӹر�\n");
			return 0;
		}
	}
	//ת�������ֽ�˳�������ֽ�˳��
	reclen = ntohl( reclen );
 
	if ( reclen > recvbuflen ) {
		//���recvbufû���㹻�Ŀռ�洢�䳤��Ϣ������ո���Ϣ�����������ش���
		while ( reclen > 0){
			iResult = recvn( s, recvbuf, recvbuflen );
			if ( iResult != recvbuflen ) {
				//����䳤��Ϣ�ڽ���ʱû�з����㹻�����ݾͷ��أ����ӹرգ���-1����������
				if ( iResult == -1 ) {
					printf("���շ�������: %d\n", WSAGetLastError());
					return -1;
				}
				else {
					printf("���ӹر�\n");
					return 0;
				}
			}
			reclen -= recvbuflen;
			//�������һ�����ݳ���
			if ( reclen < recvbuflen )
				recvbuflen = reclen;
		}
		printf("�ɱ䳤�ȵ���Ϣ����Ԥ����Ľ��ջ���\r\n");
		return -1;
	}
	//���տɱ䳤��Ϣ
	iResult = recvn( s, recvbuf, reclen );
	if ( iResult != reclen ){
		//�����Ϣ�ڽ���ʱû�з����㹻�����ݾͷ��أ����ӹرգ���-1����������
		if ( iResult == -1 ) {
			printf("���շ�������: %d\n", WSAGetLastError());
			return -1;
		}
		else {
			printf("���ӹر�\n");
			return 0;
		}
	}
	return iResult;
}
