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
	SOCKET server_socket = socket(AF_INET,SOCK_STREAM,0);    //����һ���׽���
	SOCKADDR_IN addrsrv;			//bind����Ҫ����Ϣ����Ҫ�õ�һ���ṹ�壬�����ű��ص�ַ����Ϣ��������ʹ�õ�TCPЭ�����ΪSOCKADDR_IN����
	addrsrv.sin_family = AF_INET;
	addrsrv.sin_port = htons(6000);				//ָ��Ҫ���������׽��ֵĶ˿�
	addrsrv.sin_addr.s_addr = INADDR_ANY;		//ָ��������ip��ַ
	bind(server_socket, (SOCKADDR *)&addrsrv, sizeof(SOCKADDR));
	listen(server_socket,10);					//�����˶˿�
	SOCKADDR_IN buffptr;						//����һ���ṹ�壬���ڽ��տͻ��˷��������ʱ��Я���Ŀͻ��˵ĵ�ַ��Ϣ��IP�Ͷ˿ںŵ�
	int bufflen = sizeof(SOCKADDR);
	while(TRUE)
	{
		SOCKET accept_socket = accept(server_socket,(SOCKADDR*)&buffptr,&bufflen);		//��ʼ���ϵĽ��ܿͻ�������ĵ���
		char sendbuff[100] = {0};														//���ͻ�����
		SYSTEMTIME stCurTime = {0};
		GetLocalTime(&stCurTime);
		sprintf(sendbuff,"%d/%d/%d\t%d:%d:%d\n",stCurTime.wYear,stCurTime.wMonth,stCurTime.wDay,stCurTime.wHour,stCurTime.wMinute,stCurTime.wSecond);					//������Ҫ����Ϣд�뵽���ͻ�����������inet_ntoa������buffptrָ��Ĵ洢�ͻ��˵�ַ��Ϣ�ṹ���е�sin_addrת��Ϊ���ʮ���Ƶ��ַ���
		send(accept_socket,sendbuff,sizeof(sendbuff),0);					//֮����+1����Ϊ����һ���ַ������ȥ֮��һ��Ҫ���������һ��\0�ַ���λ��
		closesocket(accept_socket);					//���δ�������������ͷ�Ϊ�˴��������������Դ���ر������׽��֡�
	}
	return 0;
}
