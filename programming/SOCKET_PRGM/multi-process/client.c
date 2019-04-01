#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main()
{

	int sockfd;

	//建立socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1)
	{
		perror("create socket failed");
		return 1;
	}


	//connect 连接server
	struct sockaddr_in serveraddr_in;
	memset(&serveraddr_in, 0, sizeof(serveraddr_in));

	serveraddr_in.sin_family = AF_INET;
	serveraddr_in.sin_port = htons(12345);   /* port in network byte order */
	serveraddr_in.sin_addr.s_addr = inet_addr("127.0.0.1");


	if ( connect(sockfd, (struct sockaddr *)&serveraddr_in, sizeof(serveraddr_in) ) == -1)
	{
		perror("connect failed");
		return 1;
	}


	char buf[100] = {0};

	while(1)
	{

		if ( fgets(buf, sizeof(buf) , stdin) == NULL )
		{
			perror("fgets failed");
			return -1;
		}
		
		write(sockfd, buf, sizeof(buf) );

		memset(buf, 0, sizeof(buf));



	}


	//close 正常关闭
	close(sockfd);

	return 0;
}
