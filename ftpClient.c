#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024

void send_file(FILE *fp, int sockfd)
{
    char data[SIZE] = {0};

    while (fgets(data, SIZE, fp) != NULL)
    {
        if (send(sockfd, data, sizeof(data), 0) == -1)
        {
            perror("[-] Error in sendung data");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

void endConnection(int sockfd)
{
    char answer[1] = {'n'};
    if (send(sockfd, answer, sizeof(char), 0) == -1)
    {
        perror("[-] Error in sending data");
        exit(1);
    }
}

int main()
{
    char *ip = "127.0.0.1";
    int port = 7777;
    int e;

    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    char filename[50];
    printf("[+] Enter file name: ");
    scanf("%s", filename);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created. \n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (e == -1)
    {
        perror("[-]Error in Connecting");
        exit(1);
    }
    printf("[+]Connected to server.\n");

    char answer = 'y';
    while (answer != 'n')
    {
        fp = fopen(filename, "r");
        if (fp == NULL)
        {
            perror("[-]Error in reading file.");
            exit(1);
        }

        send_file(fp, sockfd);
        printf("[+] File data send successfully. \n");
        fclose(fp);

        printf("[+] Send another file? (y/n) ");
        scanf("%s", &answer);
        printf("\n");

        if (answer == 'n')
        {
            endConnection(sockfd);
        }
        else if (answer == 'y')
        {
            printf("[+] Enter file name: ");
            scanf("%s", filename);
            printf("\n");
        }
        else
        {
            printf("[+] Send another file? (y/n) ");
            scanf("%s", &answer);
            printf("\n");
        }
    }
    close(sockfd);
    printf("[+]Disconnected from the server. \n");
    return 0;
}