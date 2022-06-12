#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024

void write_file(int sockfd, char *option)
{
    int n;
    FILE *fp;
    char *upload = "UPLOAD.txt";
    char *reference = "REFERENCE.txt";
    char buffer[SIZE];

    printf("%s\n", option);

    if (strcmp(&option[0], "1") == 0)
    {
        bzero(buffer, SIZE);
        printf("[+]File opened");
        fp = fopen(upload, "w");
        if (fp == NULL)
        {
            perror("[-]Error in creating file.");
            exit(1);
        }
        while (1)
        {
            n = recv(sockfd, buffer, SIZE, 0);
            if (n <= 0)
            {
                break;
                return;
            }
            fprintf(fp, "%s", buffer);
            bzero(buffer, SIZE);
        }
        fclose(fp);
        printf("[+]Data written in the text file \n");
    }
    else if (strcmp(&option[0], "2") == 0)
    {
        bzero(buffer, SIZE);

        fp = fopen(reference, "w");
        if (fp == NULL)
        {
            perror("[-]Error in creating file.");
            exit(1);
        }
        while (1)
        {
            n = recv(sockfd, buffer, SIZE, 0);
            if (n <= 0)
            {
                break;
                return;
            }
            fprintf(fp, "%s", buffer);
            bzero(buffer, SIZE);
        }
        fclose(fp);
        printf("[+]Data written in the text file \n");
    }
    else if (strcmp(&option[0], "3") == 0)
    {
        printf("[+]Connection closed. \n");
        close(sockfd);
        shutdown(sockfd, SHUT_RDWR);
        exit(1);
    }
    return;
}

int main(int argc, char **argv) {

  struct sockaddr_in servaddr;  
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(6789);

  bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));  
  listen(sock, 5);

  while(1) {
    int sockfd = accept(sock, (struct sockaddr *) NULL, NULL);

    if (sockfd >= 0) {
      char buffer[SIZE];
      int in, index = 0, limit = SIZE;

      if ((recv(sockfd, buffer, SIZE, 0)) > 0) {
        printf("%s\n", buffer);
        bzero(buffer,SIZE);    
      }
    }

    close(sockfd);
  }
}