#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <event2/event.h>


// libevent callback function: readstdin
void readstdin(evutil_socket_t fd, short what, void *arg)
{
    if (what & EV_READ)
    {
        char buf[5] = { 0 };
        read(fd, buf, sizeof(buf));
        puts(buf);
    } else {
        puts("timeout");
    }
}

// libevent callback function: wirtestdout
void writestdout(evutil_socket_t fd, short what, void *arg)
{
    if (what & EV_WRITE)
    {
        char buf[] = "!!stdout active!!\n";
        write(fd, buf, sizeof(buf));
    }
}

int main() {
    struct event_base *evbase = event_base_new();

    struct event *ev1 = event_new(evbase,
                                  STDIN_FILENO,
                                  EV_READ | EV_PERSIST,
                                  readstdin,
                                  NULL);

    struct event *ev2;

    ev2 = event_new(evbase,
                    STDIN_FILENO,
                    EV_WRITE,
                    writestdout,
                    NULL);

    struct timeval t = { 0, 0 };
    t.tv_sec = 5;
    event_add(ev1, &t);

    event_add(ev2, NULL);

    event_base_dispatch(evbase);

    event_free(ev1);
    event_base_free(evbase);

    return 0;
}
