/* UDP client in the internet domain */
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void error(const char *);
int main(int argc, char *argv[]) {
  int sock, n;
  unsigned int length;
  struct sockaddr_in server, from;
  struct hostent *hp;
  char buffer[256];

  if (argc != 3) {
    printf("Usage: server port\n");
    exit(1);
  }
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) error("socket");

  server.sin_family = AF_INET;
  hp = gethostbyname(argv[1]);
  if (hp == 0) error("Unknown host");

  bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
  server.sin_port = htons(atoi(argv[2]));
  length = sizeof(struct sockaddr_in);
  printf("Please enter the message: ");
  bzero(buffer, 256);
  fgets(buffer, 255, stdin);
  n = sendto(sock, buffer, strlen(buffer), 0, (const struct sockaddr *)&server,
             length);
  if (n < 0) error("Sendto");
  // 第四个参数是标志位，一般都设置为0
  n = recvfrom(sock, buffer, 256, 0, (struct sockaddr *)&from, &length);
  if (n < 0) error("recvfrom");
  // 0表示std_input，1表示std_output，2表示stderr
  write(1, "Got an ack: ", 12);
  write(1, buffer, n);
  close(sock);
  return 0;
}

void error(const char *msg) {
  perror(msg);
  exit(0);
}
