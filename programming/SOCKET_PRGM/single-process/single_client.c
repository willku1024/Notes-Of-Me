/* File Name: client.c */  
  
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<errno.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<netinet/in.h>  
#include<unistd.h>
#define MAXLINE 4096
#define server_port 65535  
  
void  fun()
{
        int i;
        printf("The Client is connecting to Server. Please wait:");
        for(i=0;i<=100;++i)
        {

                usleep(125555);
                printf("%3d%%\b\b\b\b\b ",i);
                fflush(stdout);


        }


        printf("\nConnect to Server successfully.\n");
}
  
int main(int argc, char** argv)  
{  
    int    sockfd, n,rec_len;  
    char    recvline[4096], sendline[4096];  
    char    buf[MAXLINE];  
    struct sockaddr_in    servaddr;  
  
  
    if( argc != 2){  
    printf("usage: ./client <Server's ipv4 address>\n");  
    exit(0);  
    }  
  
    fun();
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){  
    printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);  
    exit(0);  
    }  
  
  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_port = htons(server_port);  
    if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){  
    printf("inet_pton error for %s\n",argv[1]);  
    exit(0);  
    }  
  
  
    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){  
    printf("connect error: %s(errno: %d)\n",strerror(errno),errno);  
    exit(0);  
    }  
  
  
    printf("send msg to server: \n");  
    fgets(sendline, 4096, stdin);  
    if( send(sockfd, sendline, strlen(sendline), 0) < 0)  
    {  
    printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
    exit(0);  
    }  
    if((rec_len = recv(sockfd, buf, MAXLINE,0)) == -1) {  
       perror("recv error");  
       exit(1);  
    }  
    buf[rec_len]  = '\0';  
    printf("**NOTE** : %s \n\n",buf);  
    close(sockfd);  
    exit(0);  
}  
