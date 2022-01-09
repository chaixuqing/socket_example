#include <netdb.h>
#include <netinet/in.h>  //定义一些在Internet域中使用的常量和结构体
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>  //定义一些structures
#include <sys/types.h>   //定义了一些数据类型
#include <unistd.h>

void error(const char *msg) {
  perror(msg);  //打印错误信息到stderr
  exit(0);
}

int main(int argc, char *argv[]) {
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  /*
  struct  hostent
{
  char    *h_name;           // official name of host
  char **h_aliases;          //alias list
  int h_addrtype;             // currently always AF_INET.
  int h_length;               // length of address
  char **h_addr_list;         // list of addresses from name server
  #define h_addr h_addr_list[0] // address, for backward compatiblity
};
*/

  char buffer[256];
  if (argc < 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(0);
  }
  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) error("ERROR opening socket");
  // gethostbyname函数返回hostent结构体，其中的h_addr包含了主机的IP地址
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    fprintf(stderr, "ERROR, no such host\n");
    exit(0);
  }
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  // server->h_addr 是一个字符创，所以使用bcopy函数
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
  serv_addr.sin_port = htons(portno);
  // connnet函数用来建立与服务器的连接
  //第一个参数是socket描述符，第二个参数是一个指向struct sockaddr的指针，
  //第三个参数是结构体的大小
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR connecting");
  printf("Please enter the message: ");
  bzero(buffer, 256);
  fgets(buffer, 255, stdin);
  n = write(sockfd, buffer, strlen(buffer));
  if (n < 0) error("ERROR writing to socket");
  bzero(buffer, 256);
  n = read(sockfd, buffer, 255);
  if (n < 0) error("ERROR reading from socket");
  printf("%s\n", buffer);
  close(sockfd);
  return 0;
}
