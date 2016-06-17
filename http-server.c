#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
	int sockfd, clientfd;
	struct sockaddr_in address;
	socklen_t addrlen;

	int buffsize = 1024;
	char *buffer = malloc(buffsize);

	int PORT = 10000;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) > 0)
	{
		printf("Socket Created!\n");
	}
	else
	{
		perror("server: socket");
		exit(1);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if(bind(sockfd, (struct sockaddr *) &address, sizeof(address)) == 0)
	{
		printf("Binding Socket!\n");
	}
	else
	{
		perror("server: bind");
		exit(1);
	}

	while(1)
	{
		if(listen(sockfd, 10) < 0)
		{
			perror("server: listen");
			exit(1);
		}

		if((clientfd = accept(sockfd, (struct sockaddr *) &address, &addrlen)) < 0)
		{
			perror("server: accept");
			exit(1);
		}

		if(clientfd > 0)
		{
			printf("Client Connected!\n");
		}

		recv(clientfd, buffer, buffsize, 0);
		printf("%s\n", buffer);
		write(clientfd, "<h1>Hello!</h1>", 16);
		close(clientfd);
	}

	close(sockfd);

	return 0;
}