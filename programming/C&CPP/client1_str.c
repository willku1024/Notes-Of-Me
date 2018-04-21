#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

char name[10];
typedef struct
{
    int  fd[2];
    char timeCur[20];
    char name[20];

}clientInfo;


void getCurTime(char timeCur[])
{
    time_t  Time;
    struct tm* Tm = NULL;

    time(&Time);
    Tm = localtime(&Time);
    strftime(timeCur,20,"%Y/%m/%d %H:%M:%S",Tm);

}

void *writeFifo(void *arg)
{
    clientInfo * Client = (clientInfo *)arg;
    int fd = Client->fd[0];
    char buf[50];
    char messages[50];
	
    while(1)
    {
		memset(messages,0,sizeof(messages));
		
		int len = 0;
		len = read(STDIN_FILENO,messages,sizeof(messages));
		if(len > 0)
		{
			write(fd, messages, sizeof(messages));
		}
        else
		{
			printf("writeFifo failed %s",strerror(errno));
			break;
		}

    }
	
	close(fd);
    return NULL;
}


void *readFifo(void *arg)
{
	clientInfo * Client = (clientInfo *)arg;
    int fd = Client->fd[1];
    char buf[50];
	
    while(1)
    {
		memset(buf,0,sizeof(buf));	
		int len = 0;
		len = read(fd, buf, sizeof(buf));
		
		if(len > 0)
		{
			write(STDOUT_FILENO,buf,sizeof(buf));
				
		}
		else 
		{
			break;
		}      
        
    }

	close(fd);
    return NULL;
}

void authorInfo()
{
    puts("============================================");
    puts("Name        :ICQ in Linux");
    puts("Version     :v1.0 Beta ");
    puts("Copyright   :Power by x_jwei");
    puts("Description :A multithreaded programming in C,Ansi-style");
    puts("============================================");
}

int main(int argc,char *argv[])
{
    if(argc < 2)
    {
        printf("Usage: %s <Your Name>\n",argv[0]);
        return -1;
    }

    int fd0 = -1,fd1 = -1;

    if ( (fd0 = open("fifo0",O_RDONLY)) == -1 )
    {

        printf("open fifo0 error %s",strerror(errno));
        return -1;
    }

    if (  (fd1 = open("fifo1",O_WRONLY)) == -1  )
    {

        printf("open fifo1 error %s",strerror(errno));
        return -1;
    }

    clientInfo *client;
    client = (clientInfo *)malloc(sizeof(clientInfo));
    memset(client,0,sizeof(clientInfo));
	
    client->fd[0] = fd1;
    client->fd[1] = fd0;
    strcpy(client->name,argv[1]);

    pthread_t thr1,thr2;
    pthread_create(&thr1,NULL,writeFifo,client);
    pthread_create(&thr2,NULL,readFifo,client);
    pthread_join(thr1, NULL);
    pthread_join(thr2, NULL);


    return 0;
}


