/*tcp:server端*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int status = 1;
struct ps
{
	int st;
	pthread_t *thr;
};

void getCurTime(char timeCur[])
{
	
	time_t Time;
	struct tm *Tm = NULL;

	time(&Time);
	Tm = localtime(&Time);
	strftime(timeCur, 20, "%Y/%m/%d %H:%M:%S", Tm);
}

void catch_Signal(int Sign)
{
	switch (Sign)
	{
	case SIGINT:
		printf("\nSYS  MSG:SERVER SHUTDOWN \n");
		exit(EXIT_SUCCESS);
	}
}

int setsignal(int signo, void (*func)(int))
{
	struct sigaction act, oact;
	act.sa_handler = func;	 //回调函数初始化
	sigemptyset(&act.sa_mask); //初始化
	act.sa_flags = 0;
	return sigaction(signo, &act, &oact);
}

void *recvsocket(void *arg) //接收client端socket数据的线程
{
	struct ps *p = (struct ps *)arg;
	int st = p->st;
	char s[1024];

	while (1)
	{
		memset(s, 0, sizeof(s));
		int rc = recv(st, s, sizeof(s), 0);
		if (rc <= 0) //如果recv返回小于等于0，代表socket已经关闭或者出错了
		{
			if (rc == 0)
			{
				printf("SYS  MSG:Client Quit.\n");
				if (status != 1)
				{
					pthread_mutex_lock(&mutex);
					status--;
					printf("SYS  MSG:Online User: %d\n", status);
					pthread_mutex_unlock(&mutex);
				}
			}

			break;
		}

		struct sockaddr_in client_addr;
		char time[20];
		memset(time, 0, sizeof(time));
		memset(&client_addr, 0, sizeof(client_addr));
		socklen_t len = sizeof(client_addr);
		getpeername(st, (struct sockaddr *)&client_addr, &len);
		getCurTime(time);

		//打印client传送来的内容
		if (s[0] != 0x0d && s[0] != 0x20 && s[0] != 0x0a)
			printf("\n%s User--%s:\n%s", time, inet_ntoa(client_addr.sin_addr), s);
	}

	pthread_cancel(*(p->thr)); //被cancel掉的线程内部没有使用锁。
	return NULL;
}

void *sendsocket(void *arg) //向client端socket发送数据的线程
{
	int st = *(int *)arg;
	char s[1024];

	while (1)
	{
		memset(s, 0, sizeof(s));
		read(STDIN_FILENO, s, sizeof(s)); //从键盘读取用户输入信息
		send(st, s, strlen(s), 0);
	}
	return NULL;
}

void authorInfo()
{
	puts("============================================");
	puts("Name        : ICQ in Linux");
	puts("Version     : Beta v1.0 ");
	puts("Copyright   : Power by x_jwei");
	puts("Description : Multithread && Socket programming in C");
	puts("============================================");
}

int main(int arg, char *args[])
{
	if (arg < 2)
	{
		printf("Usage: %s  <port>\n", args[0]);
		return -1;
	}
	authorInfo();
	int port = atoi(args[1]);
	int st = socket(AF_INET, SOCK_STREAM, 0); //初始化socket

	//给TCP设置地址可重用
	int on = 1;
	if (setsockopt(st, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		printf("setsockopt failed %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	struct sockaddr_in addr; //定义一个IP地址结构
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;				  //将addr结构的属性定位为TCP/IP地址
	addr.sin_port = htons(port);			  //将本地字节顺序转化为网络字节顺序。
	addr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY代表这个server上所有的地址

	//将IP与server程序绑定
	if (bind(st, (struct sockaddr *)&addr, sizeof(addr)) == -1)
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
	int client_st = 0; //client端socket
	//socklen_t len = 0;
	struct sockaddr_in client_addr; //表示client端的IP地址
	//void *p = &client_addr;

	pthread_t thrd1, thrd2;
	setsignal(SIGINT, catch_Signal);
	printf("SYS  MSG:SERVER START UP  \n");
	while (1)
	{
		memset(&client_addr, 0, sizeof(client_addr));
		socklen_t len = sizeof(client_addr);
		//accept会阻塞，直到有客户端连接过来，accept返回client的socket描述符
		client_st = accept(st, (struct sockaddr *)&client_addr, &len);
		pthread_mutex_lock(&mutex); //为全局变量加一个互斥锁，防止与线程函数同时读写变量的冲突
		printf("SYS  MSG:User Login,Online User: %d\n", status);
		status++;
		pthread_mutex_unlock(&mutex); //解锁
		if (status > 3)				  //代表这是下一个socket连接
		{
			char tip[] = "The Number Of Users Is Limited.Please Wait.\n";
			send(client_st, tip, strlen(tip), 0);
			close(client_st);
			pthread_mutex_lock(&mutex); //为全局变量加一个互斥锁，防止与线程函数同时读写变量的冲突
			status--;
			printf("SYS  MSG:Online User: %d\n", status);
			pthread_mutex_unlock(&mutex); //解锁
			continue;
		}

		if (client_st == -1)
		{
			printf("accept failed %s\n", strerror(errno));
			return EXIT_FAILURE;
		}

		printf("SYS  MSG:Accept by client_IP:%s\n", inet_ntoa(client_addr.sin_addr));
		struct ps ps1;
		ps1.st = client_st;
		ps1.thr = &thrd2;
		pthread_create(&thrd1, NULL, recvsocket, &ps1);
		pthread_detach(thrd1);
		//设置线程为可分离
		pthread_create(&thrd2, NULL, sendsocket, &client_st);
		pthread_detach(thrd2);
		//设置线程为可分离
	}
	close(st);
	//关闭server端listen的socket

	return EXIT_SUCCESS;
}
