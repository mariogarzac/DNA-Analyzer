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
            perror("[-] Error in sending data");
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

void printMenu()
{
    printf("-------------------------------------\n");
    printf("-    Choose menu option (1, 2, 3)   -\n");
    printf("-------------------------------------\n");
    printf("- 1. Upload Sequence                -\n");
    printf("-------------------------------------\n");
    printf("- 2. Upload Reference               -\n");
    printf("-------------------------------------\n");
    printf("- 3. Exit                           -\n");
    printf("-------------------------------------\n");
    printf("\n");
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

    int menuOption;
    while (1)
    {
        printMenu();
        printf("[+]Enter menu option: ");
        scanf("%d", &menuOption);

        switch (menuOption)
        {
        case 1:
            printf("[+]Enter file name: ");
            scanf("%s", filename);

            fp = fopen(filename, "r");

            if (fp == NULL)
            {
                perror("[-]Error in reading file.");
                exit(1);
            }
            else
            {
                send_file(fp, sockfd);
                printf("[+]File data send successfully. \n");
                fclose(fp);
            }
            sleep(1);
            break;

        case 2:
            printf("[+]Enter file name: \n");
            scanf("%s", filename);

            fp = fopen(filename, "r");

            if (fp == NULL)
            {
                perror("[-]Error in reading file.");
                exit(1);
            }
            else
            {
                send_file(fp, sockfd);
                printf("[+] File data send successfully. \n");
                fclose(fp);
            }

            //TODO add recv for file matches
            sleep(1);
            break;
        case 3:
            printf("[+]Disconnected from the server. \n");
            endConnection(sockfd);
            close(sockfd);
            exit(1);
            break;
        default:
            printf("[-]Invalid choice. \n");
            sleep(.5);
        }
    }

    return 0;
}