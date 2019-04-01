#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>


/*

 struct sockaddr {
               sa_family_t sa_family;
               char        sa_data[14];
           }


 struct sockaddr_in {
               sa_family_t    sin_family;
               in_port_t      sin_port;
               struct in_addr sin_addr;
           };

 struct in_addr {
               uint32_t       s_addr;
           };



*/

void recyle(int sig)
{
    pid_t pid;
    while ( (pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        printf("pid %d recyle\n", pid);
    }
    return ;
}


int main()
{

    int serverfd;

    // create signal
    struct sigaction act;
    act.sa_handler = recyle;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    sigaction(SIGCLD, &act, NULL);




    //建立socket
    serverfd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverfd == -1)
    {
        perror("create socket failed");
        return 1;
    }

    //给TCP设置地址可重用
    int on = 1;
    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
    {
        printf("setsockopt failed %s\n", strerror(errno));
        return EXIT_FAILURE;
    }


    //bind 绑定端口，IP
    struct sockaddr_in serveraddr_in;
    memset(&serveraddr_in, 0, sizeof(serveraddr_in));

    serveraddr_in.sin_family = AF_INET;
    serveraddr_in.sin_port = htons(12345);   /* port in network byte order */
    serveraddr_in.sin_addr.s_addr = inet_addr("0.0.0.0");


    if ( bind(serverfd, (struct sockaddr *)&serveraddr_in,sizeof(serveraddr_in)) == -1)
    {
        perror("bind failed");
        return 1;
    }


    //listen 被动套接字，监听端口
    if (listen(serverfd, 20) == -1)
    {
        perror("listen failed");
        return 1;
    }


    while(1)
    {

        //accept 阻塞等待client连接

        int clientfd = -1;
        struct sockaddr_in clientaddr_in;

        memset(&clientaddr_in, 0, sizeof(clientaddr_in));
        socklen_t clientaddr_len = sizeof(clientaddr_in);
        if ( (clientfd = accept(serverfd, (struct sockaddr *)&clientaddr_in, &clientaddr_len)) < 0 )
        {

            if (errno == EINTR)
            {
                puts("inter by signal");
                continue ;
            }else {
                perror("accept failed");
                return -1;
            }
        }
        else
        {
            printf("\n\nclient %s:%d  connected\n\n",inet_ntoa(clientaddr_in.sin_addr),ntohs(clientaddr_in.sin_port));
        }


        int pid = fork();

        if(pid == 0)
        {
            //子进程关闭被动套接字
            close(serverfd);

            //业务
            while(1)
            {
                char buf[100];
                memset(buf, 0, sizeof(buf));

                int rc = -1;

                rc = read(clientfd, buf, sizeof(buf) );

                if(rc == 0)
                {
                    printf("\n\nclient %s:%d  disconn\n\n",inet_ntoa(clientaddr_in.sin_addr),ntohs(clientaddr_in.sin_port));
                    break ;
                }
                else if(rc < 0)
                {

                    printf("read failed!\n");
                }
                else
                {

                    write(1, buf, sizeof(buf));
                }

            }

        }

        if(pid > 0)
        {
            //主进程关闭一个主动套接字clientfd
            close(clientfd);

        }

    }


    //close 正常关闭
    close(serverfd);

    return 0;
}

