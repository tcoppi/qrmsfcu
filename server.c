#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

const uint32_t MAXCLIENTS = 32;

int main(int argc, char**argv)
{
    int sockfd,n;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t len;
    char mesg[1000];
    int i;

    struct sockaddr_in addresses[MAXCLIENTS];
    uint32_t bufpos = 0;

    if(argc != 2) {
        exit(1);
    }

    sockfd = socket(AF_INET,SOCK_DGRAM,0);

    memset(addresses, 0, sizeof(struct sockaddr_in) * MAXCLIENTS);
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(31000);
    bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

    for (;;) {
        len = sizeof(cliaddr);
        n = recvfrom(sockfd,mesg,500,0,(struct sockaddr *)&cliaddr,&len);

        mesg[n] = 0;

        // sub means subscribe to messages
        if(strncmp(mesg, "sub", 4)== 0) {
            addresses[bufpos % MAXCLIENTS] = cliaddr;
            bufpos++;

            sendto(sockfd, "success", 8, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
        }
        // this is a message with the key, which means to push out the "go" message
        else if(strncmp(mesg, argv[1], strlen(argv[1])) == 0) {
            printf("going\n");

            for(i=0;i<MAXCLIENTS;i++) {
                if(&addresses[i] == 0) continue;
                sendto(sockfd, "go", 3, 0, (struct sockaddr *)&addresses[i], sizeof(addresses[i]));
            }

            printf("went\n");
        }
        else {
    //        sendto(sockfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
            printf("-------------------------------------------------------\n");
            mesg[n] = 0;
            printf("Received the following:\n");
            printf("%s",mesg);
            printf("-------------------------------------------------------\n");
        }
    }
}

