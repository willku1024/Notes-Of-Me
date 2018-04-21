#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>



int main ()
{
    char buf[40];
    memset(buf,0,sizeof(buf));
    
	read(0,buf,sizeof(buf));
    write(STDOUT_FILENO,buf,sizeof(buf));
    return 0;
}
