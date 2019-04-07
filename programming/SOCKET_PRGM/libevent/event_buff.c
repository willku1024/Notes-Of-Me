#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <string.h>


// 读缓冲区回调
void read_cb(struct bufferevent *bev, void *arg)
{
    char buf[1024] = { 0 };

    bufferevent_read(bev, buf, sizeof(buf));
    char *p = "我已经收到了你发送的数据!";
    printf("client say: %s\n", buf);

    // 发数据给客户端
    bufferevent_write(bev, p, strlen(p) + 1);
    printf("====== send buf: %s\n", p);
}

// 写缓冲区回调
void write_cb(struct bufferevent *bev, void *arg)
{
    printf("我是写缓冲区的回调函数...\n");
}

// 事件
void event_cb(struct bufferevent *bev, short events, void *arg)
{
    if (events & BEV_EVENT_EOF)
    {
        printf("connection closed\n");
    }
    else if (events & BEV_EVENT_ERROR)
    {
        printf("some other error\n");
    }

    bufferevent_free(bev);
    printf("buffevent 资源已经被释放...\n");
}

void listen_cb(
    struct evconnlistener *listener,
    evutil_socket_t fd,
    struct sockaddr *addr,
    int len, void *ptr)
{
    printf("connect new client\n");

    struct event_base *base = (struct event_base *)ptr;

    // 通信操作
    // 添加新事件
    struct bufferevent *bev;
    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    // 给bufferevent缓冲区设置回调
    bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
    bufferevent_enable(bev, EV_READ);
}

int main() {
    struct event_base *evbase = event_base_new();

    struct sockaddr_in serv;

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(1234);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    struct evconnlistener *evlsn = evconnlistener_new_bind(evbase,
                                                           listen_cb,
                                                           evbase,
                                                           LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
                                                           36,
                                                           (struct sockaddr *)&serv,
                                                           sizeof(serv));


    event_base_dispatch(evbase);

    evconnlistener_free(evlsn);
    event_base_free(evbase);

    return 0;
}
