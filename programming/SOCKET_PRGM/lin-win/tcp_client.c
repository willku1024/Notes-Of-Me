#include <stdio.h>
#include <stdlib.h>
#include "tcp_pub.h"
int main(int arg, char *args[])
{
    if (arg < 4)
    {
        printf("Usage: %s  <host-ip> <host-port> <trans-filename>\n",args[0]);
        return EXIT_FAILURE;
    }
    int iport = atoi(args[2]);
    if (iport == 0)
    {
        printf("port %d is invalid\n",iport);
        return EXIT_FAILURE;
    }
    printf("%s send begin\n",args[3]);
    if (send_work(args[1],iport,args[3]) == 1)
        printf("%s send success\n",args[3]);
    else
        printf("%s send fail\n",args[3]);

    return EXIT_SUCCESS;

}