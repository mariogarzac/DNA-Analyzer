#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#include "files/process.h"

#define SIZE 1024

void write_file(int sockfd, char option)
{
    printf("[+]Data recieved.\n");
    int n;
    FILE *fp;
    char buffer[SIZE];

    if (strstr(&option, "1") != NULL)
    {
        printf("[+]Copying file contents.\n");
        fp = fopen("files/UPLOAD.txt", "w");
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
    }
    else if (strstr(&option, "2") != NULL)
    {
        printf("[+]Copying file contents.\n");
        fp = fopen("files/REFERENCE.txt", "w");
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
    }
    else if (strstr(&option, "3") != NULL)
    {
        printf("[+]Processing files... \n");
        processFile();

        // PROCESSING FUNCTION

        fp = fopen("files/RESULTS.txt", "r");

        while (fgets(buffer, SIZE, fp) != NULL)
        {
            if (send(sockfd, buffer, sizeof(buffer), 0) == -1)
            {
                perror("[-] Error in sending data");
                exit(1);
            }
            bzero(buffer, SIZE);
        }
    }
}

int main(int argc, char **argv)
{

    struct sockaddr_in servaddr;
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(6789);

    bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(sock, 5);
    printf("[+]Server created.\n");

    while (1)
    {
        printf("[+]Listening...\n");
        int sockfd = accept(sock, (struct sockaddr *)NULL, NULL);

        if (sockfd >= 0)
        {
            char buffer[SIZE];
            char option;

            if ((recv(sockfd, buffer, SIZE, 0)) > 0)
            {
                option = buffer[0];
                bzero(buffer, SIZE);

                // CLOSE SERVER CONNECTION
                if (strcmp(&option, "4") == 0)
                {
                    printf("[+]Disconnecting from server...");
                    close(sockfd);
                    shutdown(sockfd, SHUT_RDWR);
                    exit(1);
                }
                else
                {
                    write_file(sockfd, option);
                }
            }
        }
        close(sockfd);
    }
}