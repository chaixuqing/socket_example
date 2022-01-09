#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "libws2_32.lib");

DWORD WINAPI fun(void* arg);

int main() {
  WSADATA wsdata;
  if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) {
    return 0;
  }
  /*�������������׽���--IPv4Э�飬��������ͨ�ţ�TCPЭ��*/
  SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == INVALID_SOCKET) {
    printf("socket failed with error:%d\n", WSAGetLastError());
    return 1;
  }

  SOCKADDR_IN addrsrv;
  addrsrv.sin_family = AF_INET;    //����ΪIPͨ��
  addrsrv.sin_port = htons(8000);  //�������˿ں�
  addrsrv.sin_addr.s_addr =
      INADDR_ANY;  //������IP��ַ--�������ӵ����б��ص�ַ��
  /*���׽��ְ󶨵��������������ַ��*/
  if (bind(server_socket, (SOCKADDR*)&addrsrv, sizeof(SOCKADDR)) != 0) {
    printf("bind failed with error:%d\n", WSAGetLastError());
    closesocket(server_socket);
    WSACleanup();
    return 1;
  }
  /*������������--�������г���Ϊ5*/
  if (listen(server_socket, 5) != 0) {
    printf("listen failed with error:%d\n", WSAGetLastError());
    closesocket(server_socket);
    WSACleanup();
    return 1;
  }
  SOCKADDR_IN buffptr;
  int bufflen = sizeof(SOCKADDR);
  /*�ȴ��ͻ����������󵽴�*/

  while (TRUE) {
    SOCKET accept_socket = accept(server_socket, (SOCKADDR*)&buffptr, &bufflen);
    if (accept_socket == INVALID_SOCKET) {
      printf("accept failed with error:%d\n", WSAGetLastError());
      closesocket(server_socket);
      WSACleanup();
      return 1;
    }
    HANDLE hThread = CreateThread(NULL, 0, fun, (void*)&accept_socket, 0, NULL);
    CloseHandle(hThread);
  }
  printf("i have closed!\n");

  closesocket(server_socket);
  return 0;
}

DWORD WINAPI fun(void* arg) {
  SOCKET accept_socket = *(SOCKET*)arg;
  char recivebuff[1000] = {0};
  int len;
  printf("here!\n");
  /*���տͻ��˵����ݲ����䷢�͸��ͻ���--recv���ؽ��յ����ֽ�����send���ط��͵��ֽ���*/
  while ((len = recv(accept_socket, recivebuff, sizeof(recivebuff), 0)) > 0) {
    if (send(accept_socket, recivebuff, strlen(recivebuff) + 1, 0) <= 0) {
      printf("send failed with error:%d\n", WSAGetLastError());
      closesocket(accept_socket);
      WSACleanup();
    }
    printf("%s\n", recivebuff);
  }
  closesocket(accept_socket);
}
