#ifndef _H_PUB_H_
#define _H_PUB_H_

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
void getfilename(const char * filename,char *name);
int send_work(const char *hostname,int port,const char *filename);
SOCKET socket_accept (SOCKET listen_st);
SOCKET socket_create(int port);
SOCKET socket_connect(const char * hostname,int port);
SOCKET init_socket();
int recv_work(int port);

#endif