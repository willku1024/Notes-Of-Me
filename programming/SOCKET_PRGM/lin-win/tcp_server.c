#include <stdio.h>
#include <stdlib.h>
#include "tcp_pub.h"
int main(int arg, char *args[])
{
    if (arg < 2)
    {
        printf("Usage: %s <port> \n",args[0]);
        return EXIT_FAILURE;
    }
    int iport = atoi(args[1]);
    if (iport == 0)
    {
        printf("port %d is invalid\n",iport);
        return EXIT_FAILURE;
    }
    printf("%s send begin\n",args[3]);
    if (recv_work(iport) == 1)
        printf("%s recv success\n",args[3]);
    else
        printf("%s recv fail\n",args[3]);

    return EXIT_SUCCESS;

}