/*
自己动手实现一个守护进程，当控制台窗口关闭时还可以在后台运行。每隔一秒钟向
my.log文件中插入一条记录，记录格式如下:yyyy-mm-dd hh:mi:se记录内容，
其中yyyy为年，mm为月，dd为天，hh为小时，mi为分钟， se为秒。
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void WriteLog(char* content)
{
    struct tm*		tDate = NULL;
    size_t			timeLen = 0;
    time_t			tTime = 0;
    char            log[50];
    size_t          len = 0;

    memset(log,0,sizeof(log));
    tTime = time(NULL);
    tDate = localtime(&tTime);
    timeLen = strftime(log, 50, "%Y-%m-%d %H:%M:%S : ", tDate);

    len = timeLen;
    sprintf(&log[len],"%s\n",content);

    FILE * fp = fopen("log.txt","a+");
    fprintf(fp,"%s",log);
    close (fp);

    return ;
}

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if(pid == 0)
    {
        setsid();
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        while(1)
        {
            WriteLog("MyDaemon keep alived");
            sleep(1);
        }
    }
    else
    {
        exit(0);
    }

    return 0;
}
