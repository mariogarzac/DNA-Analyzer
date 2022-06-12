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

void sendFile(int sockfd)
{
    FILE *fp;
    char fileName[50];
    printf("[+]Write file name: ");
    scanf("%s", fileName);
    fp = fopen(fileName, "r");

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

void menu(int menuOption, int sockfd)
{
    FILE *fp;
    char fileName[50];

    switch (menuOption)
    {
    case 1:
        printf("[+]Write file name: ");
        scanf("%s", fileName);
        fp = fopen(fileName, "r");
        printf("Es uno!\n");
        
        fclose(fp);
        break;
    case 2:
        printf("[+]Write file name: ");
        scanf("%s", fileName);
        fp = fopen(fileName, "r");
        printf("Es dos!\n");
        fclose(fp);
        break;
    case 3:
        printf("[+]Closing connection.");
        close(sockfd);
        shutdown(sockfd, SHUT_RDWR);
        exit(1);
    default:
        printf("[-]Invalid option. \n");
        break;
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
        scanf("%d", &option);

        charValue[0] = option + '0';
        charValue[1] = '\0';

        if (strcmp(charValue, "3") == 0){
            printf("[+]Closing connection.");
            sendOption(servaddr, sockfd, charValue);
            close(sockfd);
            shutdown(sockfd, SHUT_RDWR);
            exit(1);
        }

        // SEND MENU OPTION
        sendOption(servaddr, sockfd, charValue);

        // SEND FILE
        sendFile(sockfd);
        //menu(option, sockfd);

        // CLOSE SOCKET TO BEGIN NEW CONNECTION
        bzero(charValue, 2);
        close(sockfd);
    }
}