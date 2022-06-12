#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024

void sendOption(struct sockaddr_in servaddr, int sockfd, char *charValue)
{
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    send(sockfd, charValue, 2 * sizeof(char), 0);
}

void printMenu()
{
    printf("-------------------------------------\n");
    printf("-  Choose menu option (1, 2, 3, 4)  -\n");
    printf("-------------------------------------\n");
    printf("- 1. Upload Sequence                -\n");
    printf("-------------------------------------\n");
    printf("- 2. Upload Reference               -\n");
    printf("-------------------------------------\n");
    printf("- 3. Compare files                  -\n");
    printf("-------------------------------------\n");
    printf("- 4. Exit                           -\n");
    printf("-------------------------------------\n");
    printf("\n");
}

void sendFile(int sockfd)
{
    FILE *fp;
    char fileName[50];
    char path[50] = "files/";
    printf("[+]Write file name: ");
    scanf("%s", fileName);
    strcat(path, fileName);

    fp = fopen(path, "r");

    if (fp != NULL)
    {
        char data[SIZE] = {0};
        while (fgets(data, SIZE, fp) != NULL)
        {
            if (send(sockfd, data, sizeof(data), 0) == -1)
            {
                perror("[-]Error in sending data");
                exit(1);
            }
            bzero(data, SIZE);
        }
        fclose(fp);
    }
    else
    {
        perror("[-]Error opening file.\n");
    }
}

void recieveComparison(int sockfd){
    int n;
    char buffer[SIZE];
    while (1)
        {
            n = recv(sockfd, buffer, SIZE, 0);
            if (n <= 0)
            {
                break;
                return;
            }
            printf("%s", buffer);
            bzero(buffer, SIZE);
        }
}


int main(int argc, char **argv)
{

    struct sockaddr_in servaddr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6789);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1)
    {
        // CREATE NEW SOCKET CONNECTION
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        int option = 0;
        char charValue[2];

        // PRINT OPTION MENU
        printMenu();

        // ASK FOR MENU OPTION
        fflush(stdin);
        printf("[+]Select option: ");
        scanf("%d", &option);

        charValue[0] = option + '0';
        charValue[1] = '\0';

        sendOption(servaddr, sockfd, charValue);
        if ((strcmp(charValue, "1") == 0) || strcmp(charValue, "2") == 0)
        {
            sendFile(sockfd);
        }
        else if (strcmp(charValue, "3") == 0)
        {
            printf("[+]Waiting for resutls...");
            recieveComparison(sockfd);
        }
        else if (strcmp(charValue, "4") == 0)
        {
            printf("[+]Closing connection.\n");
            close(sockfd);
            shutdown(sockfd, SHUT_RDWR);
            exit(1);
        }

        // CLOSE SOCKET TO BEGIN NEW CONNECTION
        bzero(charValue, 2);
        close(sockfd);
    }
}