#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t pid;
    pid = fork();

    if(pid == 0)
    {
        int i;
        for(i = 1; i <= 100; i++ )
        {

            pid_t Pid;
            Pid = fork();
            if(Pid == 0)
            {
                exit(0);
            }
    
        }

    }
    else
    {
 
        exit(0);
    }


    return 0;
}
