#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/select.h>
#include <arpa/inet.h>
#include <errno.h>

#define LISTEN_BACKLOG 50
#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)


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


    fd_set rfds, tmpfds;

    FD_ZERO(&rfds);


    FD_SET(sfd, &rfds);

    int maxfd = sfd;


    while (1)
    {
        tmpfds = rfds;
        puts("select wait:");
        int retval = select(maxfd + 1, &tmpfds, NULL, NULL, NULL);

        if (retval == -1)
        {
            perror("select()");
            break;
        } else if (retval)
        {

            if (FD_ISSET(sfd, &tmpfds))
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
                FD_SET(cfd, &rfds);
                // update max fd
                maxfd = maxfd > cfd ? maxfd : cfd;

            }

            for (int i = sfd + 1; i < maxfd + 1; ++i)
            {
                if (FD_ISSET(i, &tmpfds))
                {
                    // do some sth
                    char buf[1024] = {0};
                    int n = 0;
                    while ((n = read(i, buf, 1)))
                    {
                        if (n > 0)
                        {
                            printf("%s", buf);
                        }
                        else
                        {
                            // no data can read
                            if(errno == EAGAIN || errno == EWOULDBLOCK)
                                break;
                            else
                                puts("read error");
                        }

                    }

                    if (n == 0)
                    {
                        printf("read EOF, client quit\n");
                        deactive_nonblock(i);
                        FD_CLR(i, &rfds);
                        close(i);
                    }
                }
            }


        } else
        {
            // printf("No data within five seconds.\n");
            continue;
        }


    }


    return 0;
}