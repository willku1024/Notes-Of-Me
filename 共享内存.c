#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char * argv[])
{

    char *shmbuf;
    int shmid = 0;
    shmbuf = shmat(shmid,0,0);
    pid_t pid = fork();
    if (pid == 0)
    {


        printf("please input:\n");
        scanf("%s",shmbuf);


    }
    else
    {

        sleep(4);
        printf("rev: %s\n",shmbuf);

    }
    return 0;
}
