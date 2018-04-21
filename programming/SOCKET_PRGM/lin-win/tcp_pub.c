#ifdef WIN
#include <WinSock2.h>
#else
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#define SOCKET int
#endif

#include <stdio.h>
#include "tcp_pub.h"

#define BUFSIZE 262144//256k

void getfilename(const char * filename,char *name)
{
    int len = strlen(filename);
    int i;
    for (i = (len -1);i >=0;i--)
    {
        if ((filename[i] == '\\') || (filename[i]) == '/')
        {
            break;
        }
    }
    strcpy(name,&filename[i + 1]);
    return ;
}

SOCKET init_socket()//初始化socket
{
//如果是windows,执行如下代码
#ifdef WIN
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(1,1);//获取版本
    err = WSAStartup(wVersionRequested,&wsaData);
    if (err != 0)
    {
        return -1;
    }

    if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
    {
        WSACleanup();
        return -1;
    }
#endif
//如果是linux,直接返回
    return 0;

}

SOCKET socket_connect(const char * hostname,int port)//连接到指定的主机和端口号
{
    if (init_socket() == -1)
        return 0;

    SOCKET st = socket(AF_INET,SOCK_STREAM,0);
    if (st == 0)
        return 0;
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(hostname);
    if (connect(st,(struct sockaddr *)&addr,sizeof(addr)) == -1)
    {
        printf("connect to %s:%d failed %s\n",hostname,port,strerror(errno));
        return 0;//连接失败
    } else
    {
        return st;//连接成功,返回socket描述符
    }
}


SOCKET socket_create(int port)//在port指定端口上建立server端的socket
{
    if (init_socket() == -1)
        return 0;

    SOCKET st = socket(AF_INET,SOCK_STREAM,0);
    if (st == 0)
        return 0;
#ifdef WIN
    const char on =0;
#else
    int on = 0;
#endif

    if (setsockopt(st, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
    {
        printf("setsockopt failed %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr;//定义一个IP地址结构
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;//将addr结构的属性定位为TCP/IP地址
    addr.sin_port = htons(port);//将本地字节顺序转化为网络字节顺序。
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //将IP与server程序绑定
    if (bind(st, (struct sockaddr *) &addr, sizeof(addr)) == -1)
    {
        printf("bind failed %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    //server端开始listen，
    if (listen(st, 20) == -1)
    {
        printf("listen failed %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    printf("listen %d success\n",port);
    return st;


}


SOCKET socket_accept (SOCKET listen_st)
{

    struct sockaddr_in client_addr;
#ifdef WIN
    int len = 0;
#else
    unsigned int len = 1;
#endif

    len = sizeof(client_addr);
    memset(&client_addr, 0,sizeof(client_addr));
    SOCKET client_st = accept(listen_st,(struct sockaddr *)&client_addr,&len);
    if (client_st == -1)
    {

        printf("accept failed %s\n",strerror(errno));
        return 0;
    }
    else
    {
        printf("accept by %s\n",inet_ntoa(client_addr.sin_addr));
        return client_st;
    }


}


int send_work(const char *hostname,int port,const char *filename)
{
    SOCKET st = socket_connect(hostname,port);//链接到指定的IP和port
    if (st == 0)
        return 0;

    FILE *fd = fopen(filename,"rb");//只读方式打开
    if (fd == NULL)
    {
        printf("open %s failed %s\n",filename,strerror(errno));
        return 0;
    }

    char *buf = malloc(BUFSIZE);
    memset(buf,0,BUFSIZE);
    getfilename(filename,buf);//从完整路径中解析文件名
    size_t rc = send(st,buf,strlen(buf),0);//客户端第一次发送的是文件名
    if (rc <= 0)
    {
        if (rc < 0 )
            printf("send failed %s\n",strerror(errno));
        else
            printf("socket disconnect\n");
    } else
    {
        memset(buf,0,BUFSIZE);
        if ((rc = recv(st, buf, BUFSIZE, 0))<= 0)//接受来自server的回复
        {
            if (rc < 0)
                printf("recv failed %s\n",strerror(errno));
            else
                printf("socket disconnect\n");

        } else
        {
            if (strncmp(buf,"OK",2) ==0)//如果收到服务端的回复,代表服务端认可,可以发送文件了
            {
                while (1)
                {
                    memset(buf,0,BUFSIZE);
                    rc = fread(buf,1,BUFSIZE,fd);//循环读取,直到文件尾部
                    if (rc <= 0)
                    {
                        if (rc < 0)
                            printf("send failed %s\n",strerror(errno));
                        break;

                    } else
                    {
                        rc = send(st,buf,rc,0);
                        if (rc <= 0)//发送失败,代表TCP连接出错,循环break
                        {
                            if (rc < 0)
                                printf("send failed %s\n",strerror(errno));
                            else
                                printf("socket disconnect\n");
                            break;
                        }
                    }
                }
            }
        }
    }

    fclose(fd);
    free(buf);

#ifdef WIN
    closesocket(st);
    WSACleanup();
#else
    close(st);
#endif
    return 1;

}





int recv_work(int port)
{

    SOCKET listen_st = socket_create(port);
    if (listen_st == 0)
        return 0;
    SOCKET st = socket_accept(listen_st);
    if (st == 0)
        return 0;

    char *buf = malloc(BUFSIZE);
    FILE *fd = NULL;

    memset(buf, 0,BUFSIZE);
    size_t rc = recv(st,buf,BUFSIZE,0);
    if (rc <= 0)
    {
        if (rc < 0)
            printf("recv failed %s\n",strerror(errno));
        else
            printf("socket disconnen\n");

    } else
    {
        printf("receiving %s\n",buf);

        fd = fopen(buf,"wb");
        if (fd == NULL)
        {
            printf("open %s failed %s\n",buf,strerror(errno));
        } else
        {
            memset(buf,0,BUFSIZE);
            strcpy(buf,"OK");
            rc = send(st,buf,strlen(buf),0);
            if (rc <= 0)
            {
                if (rc < 0)
                    printf("send failed %s",strerror(errno));
                else
                    printf("socket disconnect\n");
            }

            while (1)
            {
                memset(buf,0,BUFSIZE);
                rc = recv(st,buf,BUFSIZE,0);
                if (rc <=0 )
                {
                    if (rc < 0)
                        printf("recv failed %s\n",strerror(errno));
                    else
                        printf("socket disconnect\n");
                    break;

                } else
                {
                    fwrite(buf,1,rc,fd);
                }
            }
        }
    }


    if (fd)
        fclose(fd);
    free(buf);
#ifdef WIN
    closesocket(st);
    closesocket(listen_st);
    WSACleanup();
#else
    close(st);
    close(listen_st);
#endif

    return 1;


}











