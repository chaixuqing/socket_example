/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <netinet/in.h>  //定义一些在Internet域中使用的常量和结构体
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>  //定义一些structures
#include <sys/types.h>   //定义了一些数据类型
#include <unistd.h>

void error(const char *msg) {
  perror(msg);  //打印错误信息到stderr
  exit(1);
}

int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno;  //两个文件描述符和一个端口号
  socklen_t clilen;               //客户端地址长度
  char buffer[256];               //接收缓冲区
  struct sockaddr_in serv_addr, cli_addr;
  /*
  struct sockaddr_in   // Internet socket address structure
  {
    short   sin_family;   // Address family, must be AF_INET
    u_short sin_port;     // Port number
    struct  in_addr sin_addr;  // Internet address, 包含一个s_addr成员
    char    sin_zero[8];  //  Not used, must be zero
  }; */
  int n;
  if (argc < 2) {  //参数需要包括端口号
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }
  // 第一个参数是socket的address domain 即AF_INET，表示Internet域
  // 第二个参数是socket的type，即SOCK_STREAM
  // 第三个参数是传输协议，即0，操作系统会选择合适的传输协议
  // 根据type选择TCP或者UDP
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  // If the socket call fails, it returns -1.
  if (sockfd < 0) error("ERROR opening socket");
  bzero((char *)&serv_addr,
        sizeof(serv_addr));  //将结构体serv_addr的所有成员清零
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  // INADDR_ANY表示所有本地地址，也就是任何一个网卡上的请求都可以接受
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  // htons()函数将一个无符号短整型的主机字节序转换成网络字节序
  serv_addr.sin_port = htons(portno);
  // bind()函数将一个套接字绑定到本地地址
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");
  //开始监听，同时设置队列的长度
  listen(sockfd, 5);
  clilen = sizeof(cli_addr);
  // accept()函数阻塞返回一个新的套接字，cli_addr是客户端的IP地址
  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
  if (newsockfd < 0) error("ERROR on accept");
  bzero(buffer, 256);
  n = read(newsockfd, buffer, 255);  //阻塞读取数据
  if (n < 0) error("ERROR reading from socket");
  printf("Here is the message: %s\n", buffer);
  n = write(newsockfd, "I got your message", 18);  //最后一个参数是写入的字节数
  if (n < 0) error("ERROR writing to socket");
  close(newsockfd);
  close(sockfd);
  return 0;
}
