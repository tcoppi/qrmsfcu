#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char**argv)
{
    int sockfd,n;
    struct sockaddr_in servaddr,cliaddr;
    char sendline[1000];
    char recvline[1000];

    if (argc != 3) {
       exit(1);
    }

    sockfd = socket(AF_INET,SOCK_DGRAM,0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port=htons(31000);
/*
    while (fgets(sendline, 500,stdin) != NULL) {
        sendto(sockfd,sendline,strlen(sendline),0,
              (struct sockaddr *)&servaddr,sizeof(servaddr));
        n = recvfrom(sockfd,recvline,500,0,NULL,NULL);
        recvline[n] = 0;
        fputs(recvline,stdout);
    }
    */
    for (;;) {
        n = recvfrom(sockfd, recvline, 500, 0, NULL, NULL);

        if(strncmp(recvline, "go", 3) == 0) {
            system(argv[2]);
            printf("went!\n");
            sendto(sockfd, "went", 5, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        }
    }
}

