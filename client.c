#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024

void sendOption(struct sockaddr_in servaddr, int sockfd, char *charValue){
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    send(sockfd, charValue, 2 * sizeof(char), 0);
}

int main(int argc, char **argv) {

  struct sockaddr_in servaddr;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(6789);
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");


  while(1) {
    // CREATE NEW SOCKET CONNECTION
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int option = 0;

    // ASK FOR MENU OPTION
    fflush(stdin);
    scanf("%d", &option);
    char charValue[2];
    charValue[0] = option + '0';
    charValue[1] = '\0';

    if (strcmp(charValue, "1") == 0){
        printf("Son iguales!");
    }

    // SEND MENU OPTION
    sendOption(servaddr, sockfd, charValue);

    // CLOSE SOCKET TO BEGIN NEW CONNECTION
    close(sockfd);
  }
}