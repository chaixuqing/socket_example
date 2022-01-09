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
  /*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
  SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == INVALID_SOCKET) {
    printf("socket failed with error:%d\n", WSAGetLastError());
    return 1;
  }

  SOCKADDR_IN addrsrv;
  addrsrv.sin_family = AF_INET;    //设置为IP通信
  addrsrv.sin_port = htons(8000);  //服务器端口号
  addrsrv.sin_addr.s_addr =
      INADDR_ANY;  //服务器IP地址--允许连接到所有本地地址上
  /*将套接字绑定到服务器的网络地址上*/
  if (bind(server_socket, (SOCKADDR*)&addrsrv, sizeof(SOCKADDR)) != 0) {
    printf("bind failed with error:%d\n", WSAGetLastError());
    closesocket(server_socket);
    WSACleanup();
    return 1;
  }
  /*监听连接请求--监听队列长度为5*/
  if (listen(server_socket, 5) != 0) {
    printf("listen failed with error:%d\n", WSAGetLastError());
    closesocket(server_socket);
    WSACleanup();
    return 1;
  }
  SOCKADDR_IN buffptr;
  int bufflen = sizeof(SOCKADDR);
  /*等待客户端连接请求到达*/

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
  /*接收客户端的数据并将其发送给客户端--recv返回接收到的字节数，send返回发送的字节数*/
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
