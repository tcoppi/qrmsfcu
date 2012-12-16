#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

double get_time(void)
{
   struct timeval      timev;

   gettimeofday(&timev, NULL);
   return (double)timev.tv_sec + (((double)timev.tv_usec) / 1000000);
}


int main(int argc, char**argv)
{
    int sockfd,n;
    struct sockaddr_in servaddr,cliaddr;
    char sendline[1000];
    char recvline[1000];
    double t1, t2, t;

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

    // send sub message
    t1 = get_time();
    sendto(sockfd, "sub", 3, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    for (;;) {
        n = recvfrom(sockfd, recvline, 500, 0, NULL, NULL);
        recvline[n] = 0;


        if(strncmp(recvline, "go", 3) == 0) {
            system(argv[2]);
            printf("went!\n");
            sendto(sockfd, "went", 5, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        }
        else if(strncmp(recvline, "success", 8) == 0) {
            t2 = get_time();
            t = t2 - t1;
            printf("latency: %3.3f seconds\n", t);
            printf("successfully subscribed to server\n");
        }
    }
}

