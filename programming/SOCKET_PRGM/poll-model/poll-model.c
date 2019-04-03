/*
IO多路转接 - poll
poll结构体: 
struct pollfd {
int fd; //文件描述符
short events;  // 需要监听的事件
short revents; // 内核给的反馈，实际发生的事件
};

函数原型:
- int select(int nfds,
fd_set *readfds, fd_set *writefds,
fd_set *exceptfds,
struct timeval *timeout);
- int poll(struct pollfd *fd, nfds_t nfds, int timeout);
○ pollfd -- 数组的地址
○ nfds: 数组的最大长度, 数组中最后一个使用的元素下标+1
     内核会轮询检测fd数组的每个文件描述符 ○ timeout:
     -1: 永久阻塞
     0: 调用完成立即返回  >0: 等待的时长毫秒
○ 返回值: iO发送变化的文件描述符的个数

*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <errno.h>

#define LISTEN_BACKLOG 50
#define handle_error(msg)   \
    do                      \
    {                       \
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

    if (bind(sfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in)) == -1)
        handle_error("bind");

    if (listen(sfd, LISTEN_BACKLOG) == -1)
        handle_error("listen");

    //struct pollfd {
    //    int   fd;         /* file descriptor */
    //    short events;     /* requested events */
    //    short revents;    /* returned events */
    //};

    struct pollfd allfd[1024];

    for (int j = 0; j < 1024; ++j)
    {
        allfd[j].fd = -1;
        allfd[j].events = POLLIN;
    }

    allfd[0].fd = sfd;
    int maxfd_idx = 0;

    while (1)
    {
        // int poll(struct pollfd *fds, nfds_t nfds, int timeout);
        // timewait: -1:block 0:call return >0:set timeout

        puts("poll wait:");
        int retval = poll(allfd, maxfd_idx + 1, -1);

        if (retval == -1)
        {
            perror("select()");
            break;
        }

        if (allfd[0].revents & POLLIN)
        {
            puts("listen active");
            struct sockaddr_in peer_addr;
            socklen_t peer_addr_size;
            peer_addr_size = sizeof(struct sockaddr_in);
            cfd = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addr_size);

            if (cfd == -1)
                handle_error("accept");

            char ip[INET_ADDRSTRLEN] = {0};
            printf("cli-ip:%s cli-port:%d conn server.\n",
                   inet_ntop(AF_INET, &peer_addr.sin_addr, ip, sizeof(ip)), ntohs(peer_addr.sin_port));

            active_nonblock(cfd);

            int i = 1;
            // cfd添加到ｐｏｌｌ数组
            for (; i < 1024; ++i)
            {
                if (allfd[i].fd == -1)
                {
                    allfd[i].fd = cfd;
                    break;
                }
            }

            // update max fd
            maxfd_idx = maxfd_idx > i ? maxfd_idx : i;
        }

        for (int i = 1; i < maxfd_idx + 1; ++i)
        {
            int fd = allfd[i].fd;
            if (fd == -1)
                continue;

            if (allfd[i].revents & POLLIN)
            {
                // do some sth
                char buf[1024] = {0};
                int n = 0;
                while ((n = read(fd, buf, 1)))
                {
                    if (n > 0)
                    {
                        printf("%s", buf);
                    }
                    else
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
                    allfd[i].fd = -1;
                    close(i);
                }

                // process some things
                char sendbuf[] = "srever send data";
                write(fd, sendbuf, sizeof(sendbuf));
            }
        }
    }

    return 0;
}