/*tcp:server��*/
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
struct ps{
	int st;
	pthread_t *thr;
};

void getCurTime(char timeCur[])
{
    time_t  Time;
    struct tm* Tm = NULL;

    time(&Time);
    Tm = localtime(&Time);
    strftime(timeCur,20,"%Y/%m/%d %H:%M:%S",Tm);

}

void catch_Signal(int Sign)
{
	switch(Sign)
	{
		case SIGINT:
			printf("\nSYS  MSG:SERVER SHUTDOWN \n");
			exit(EXIT_SUCCESS);
	}
}

int signal1(int signo,void (*func)(int))
{
	struct sigaction act,oact;
	act.sa_handler = func;//�ص�������ʼ��
	sigemptyset(&act.sa_mask);//��ʼ��
	act.sa_flags = 0;
	return sigaction(signo,&act,&oact);
	
}

void *recvsocket(void *arg)//����client��socket���ݵ��߳�
{
	struct ps *p = (struct ps *)arg;
	int st = p->st;
	char s[1024];
	
	while(1)
	{
		memset(s, 0, sizeof(s));
		int rc = recv(st, s, sizeof(s), 0);
		if (rc <= 0)//���recv����С�ڵ���0������socket�Ѿ��رջ��߳�����
        {
			if(rc == 0)
			{
				 printf("SYS  MSG:Client Quit.\n");
				if(status != 1)
				{
					pthread_mutex_lock(&mutex);
					status-- ;printf("SYS  MSG:Online User: %d\n",status);
					pthread_mutex_unlock(&mutex);	
				}
			}
	
            break;
        }
        
		struct sockaddr_in client_addr;
		char time[20];
		memset(time, 0, sizeof(time));
		memset(&client_addr,0,sizeof(client_addr));
		socklen_t len = sizeof(client_addr);
		getpeername(st, (struct sockaddr *) &client_addr, &len);
		getCurTime(time);
		
		//��ӡclient������������
		if(s[0] != 0x0d && s[0] != 0x20 && s[0] != 0x0a)
			printf("\n%s User--%s:\n%s",time, inet_ntoa(client_addr.sin_addr),s);

	}

	pthread_cancel(*(p->thr));//��cancel�����߳��ڲ�û��ʹ������
	return NULL;
}

void *sendsocket(void *arg)//��client��socket�������ݵ��߳�
{
	int st = *(int *)arg;
	char s[1024];

	while(1)
	{
		memset(s, 0, sizeof(s));
		read(STDIN_FILENO, s, sizeof(s));//�Ӽ��̶�ȡ�û�������Ϣ
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
		printf("Usage: %s  <port>\n",args[0]);
		return -1;
	}
    authorInfo();
	int port = atoi(args[1]);
	int st = socket(AF_INET, SOCK_STREAM, 0);//��ʼ��socket

    //��TCP���õ�ַ������
	int on = 1;
	if (setsockopt(st, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		printf("setsockopt failed %s\n", strerror(errno));
		return EXIT_FAILURE;
	}


	struct sockaddr_in addr;//����һ��IP��ַ�ṹ
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;//��addr�ṹ�����Զ�λΪTCP/IP��ַ
	addr.sin_port = htons(port);//�������ֽ�˳��ת��Ϊ�����ֽ�˳��
	addr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY�������server�����еĵ�ַ

	//��IP��server�����
	if (bind(st, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	{
		printf("bind failed %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	//server�˿�ʼlisten��
	if (listen(st, 20) == -1)
	{
		printf("listen failed %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	int client_st = 0;//client��socket
	//socklen_t len = 0;
	struct sockaddr_in client_addr;//��ʾclient�˵�IP��ַ
	//void *p = &client_addr;

	pthread_t thrd1, thrd2;
    signal1(SIGINT,catch_Signal);
	printf("SYS  MSG:SERVER START UP  \n");
	while (1)
	{
		memset(&client_addr, 0, sizeof(client_addr));
		socklen_t len = sizeof(client_addr);
		//accept��������ֱ���пͻ������ӹ�����accept����client��socket������
		client_st = accept(st, (struct sockaddr *)&client_addr , &len);
		pthread_mutex_lock(&mutex);//Ϊȫ�ֱ�����һ������������ֹ���̺߳���ͬʱ��д�����ĳ�ͻ
		printf("SYS  MSG:User Login,Online User: %d\n",status);status++; 
		pthread_mutex_unlock(&mutex);//����
		if (status > 3)//����������һ��socket����
		{
		    char tip[] = "The Number Of Users Is Limited.Please Wait.\n";
		    send(client_st, tip, strlen(tip), 0);
			close(client_st);
			pthread_mutex_lock(&mutex);//Ϊȫ�ֱ�����һ������������ֹ���̺߳���ͬʱ��д�����ĳ�ͻ
			status--; printf("SYS  MSG:Online User: %d\n",status);
			pthread_mutex_unlock(&mutex);//����
			continue;
		}

		if (client_st == -1)
		{
			printf("accept failed %s\n", strerror(errno));
			return EXIT_FAILURE;
		}

		printf("SYS  MSG:Accept by client_IP:%s\n",inet_ntoa(client_addr.sin_addr));
		struct ps ps1;
		ps1.st = client_st;
		ps1.thr = &thrd2;
		pthread_create(&thrd1, NULL, recvsocket, &ps1);
		pthread_detach(thrd1);
		//�����߳�Ϊ�ɷ���
		pthread_create(&thrd2, NULL, sendsocket, &client_st);
		pthread_detach(thrd2);
		//�����߳�Ϊ�ɷ���
	}
	close(st);
	//�ر�server��listen��socket
	
	return EXIT_SUCCESS;
}
