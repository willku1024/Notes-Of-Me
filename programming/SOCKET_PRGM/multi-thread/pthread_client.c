/*tcp:client��*/
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

void *recvsocket(void *arg)
{
	int st = *(int *)arg;
	char s[1024];

	while(1)
	{
		memset(s, 0, sizeof(s));
		int rc = recv(st, s, sizeof(s), 0);
		if (rc <= 0)
		{
			if(rc == 0)
			{
				printf("Server Quit!\n");
			}
			break;
		}
			
		printf("%s\n", s);

	}
	return NULL;
}

void *sendsocket(void *arg)
{
	int st = *(int *)arg;
	char s[1024];

	while(1)
	{
		memset(s, 0, sizeof(s));
		read(STDIN_FILENO, s, sizeof(s));
		send(st, s, strlen(s), 0);
	}
	return NULL;
}



int main(int arg, char *args[])
{
	if (arg < 3)
	{
		printf("Usage: %s  <server-IP> <server-port>\n",args[0]);
		return -1;
	}
		

	int port = atoi(args[2]);
	int st = socket(AF_INET, SOCK_STREAM, 0); //��ʼ��socket��

	struct sockaddr_in addr; //����һ��IP��ַ�Ľṹ
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET; //���ýṹ��ַ����ΪTCP/IP��ַ
	addr.sin_port = htons(port); //ָ��һ���˿ںţ�8080��htons:��short���ʹ�host�ֽ����͵�net�ֽ�����ת��
	addr.sin_addr.s_addr = inet_addr(args[1]); //���ַ������͵�IP��ַת��Ϊint������addr�ṹ��Ա.

	//����connect���ӵ��ṹaddrָ����IP��ַ�Ͷ˿ں�
	if (connect(st, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	{
		printf("connect failed %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	pthread_t thrd1, thrd2;
	pthread_create(&thrd1, NULL, recvsocket, &st);
	pthread_create(&thrd2, NULL, sendsocket, &st);
	pthread_join(thrd1, NULL);
	//pthread_join(thrd2, NULL);
	close(st); //�ر�socket
	return EXIT_SUCCESS;

}
