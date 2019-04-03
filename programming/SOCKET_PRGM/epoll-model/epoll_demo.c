/*
epoll三个函数:
1、该函数生成一个epoll专用的文件描述符
○ int epoll_create(int size);
     size: epoll上能关注的最大描述符数

2、用于控制某个epoll文件描述符事件，可以注 册、修改、删除
○ int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
     epfd: epoll_create生成的epoll专用描述符 
     op:
        □ EPOLL_CTL_ADD -- 注册 
        □ EPOLL_CTL_MOD -- 修改 
        □ EPOLL_CTL_DEL -- 删除
     fd: 关联的文件描述符
     event: 告诉内核要监听什么事件 - 等待IO事件发生 - 可以设置阻塞的函数
    typedef union epoll_data {
        void        *ptr;
        int          fd;
        uint32_t     u32;
        uint64_t     u64;
    } epoll_data_t;

    struct epoll_event {
        uint32_t     events;      // Epoll events: EPOLLIN - 读、EPOLLOUT - 写、EPOLLERR - 异常
        epoll_data_t data;        // User data variable 
    };

3、int epoll_wait(
    int epfd,   // epoll_create 创建的句柄
    struct epoll_event* events, // 数组 
    int maxevents,
    int timeout
);
     epfd: 要检测的句柄
     events:用于回传待处理事件的数组
     maxevents:告诉内核这个events的大小 
     timeout:为超时时间
        □ -1: 永久阻塞 
        □ 0: 无活跃fd，立即返回 
        □ >0: 活跃fd数目
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/epoll.h>

#define LISTEN_BACKLOG 50
#define handle_error(msg)   \
    do                      \
    {                       \
        printf("%d,",__LINE__);\
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

//设置socket非阻塞
void active_nonblock(int fd)
{
    int ret;
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        handle_error("fcntl");

    flags |= O_NONBLOCK;
    ret = fcntl(fd, F_SETFL, flags);
    if (ret == -1)
        handle_error("fcntl");
}

//设置socket阻塞
void deactive_nonblock(int fd)
{
    int ret;
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        handle_error("de-fcntl");

    flags &= ~O_NONBLOCK;
    ret = fcntl(fd, F_SETFL, flags);
    if (ret == -1)
        handle_error("de-fcntl");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("usage:%s [port]\n", argv[0]);
        return 1;
    }

    int sfd, cfd;

    struct sockaddr_in my_addr;

    memset(&my_addr, 0, sizeof(struct sockaddr_in));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_port = htons(atoi(argv[1]));

    // create
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
        handle_error("socket");

    int on = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    if (bind(sfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_in)) == -1)
        handle_error("bind");

    if (listen(sfd, LISTEN_BACKLOG) == -1)
        handle_error("listen");


    int epfd = epoll_create(10);

    struct epoll_event ev, events[200];
    ev.events = EPOLLIN;
    ev.data.fd = sfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &ev);

    while (1)
    {
        int ret = epoll_wait(epfd, events, sizeof(events) / sizeof(events[0]), -1);
        if (ret == -1)
            handle_error("epoll_wait err");

        for (int i = 0; i < ret; ++i)
        {
            int fd = events[i].data.fd;
            if (fd == sfd)
            {
                struct sockaddr_in peer_addr;
                socklen_t peer_addr_size;
                peer_addr_size = sizeof(struct sockaddr_in);
                cfd = accept(sfd, (struct sockaddr *) &peer_addr, &peer_addr_size);

                if (cfd == -1)
                    handle_error("accept");

                char ip[INET_ADDRSTRLEN] = {0};
                printf("cli-ip:%s cli-port:%d conn server.\n",
                       inet_ntop(AF_INET, &peer_addr.sin_addr, ip, sizeof(ip)), ntohs(peer_addr.sin_port));

                active_nonblock(cfd);

                ev.events = EPOLLIN;
                ev.data.fd = cfd;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev) < 0)
                    handle_error("epoll_ctl error");
            } else
            {
                // can read event
                if (events[i].events & EPOLLIN)
                {

                    // do some sth
                    char buf[2] = {0};
                    int n = 0;
                    while ((n = read(fd, buf, 1)))
                    {

                        if (n > 0)
                        {
                            printf("%s", buf);
                        } else
                        {
                            // no data can read
                            if (errno == EAGAIN || errno == EWOULDBLOCK)
                                break;
                            else
                                puts("read error");
                        }
                    }

                    if (n == 0)
                    {
                        printf("read EOF, client quit\n");
                        deactive_nonblock(i);
                        if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) < 0)
                            handle_error("epoll_ctl error");
                        close(i);
                    }
                }
            }

        }
    }

    return 0;
}