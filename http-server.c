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
#include <netdb.h>
#include <signal.h>
#include <fcntl.h>

#include "response.h"

int PORT = 10000;
char * ROOT;

int main(int argc, char **argv)
{
	int sock_fd, clientfd;
	struct sockaddr_in address;
	socklen_t addrlen;

	int buffsize = 1024;
	char *buffer = malloc(buffsize);

	char index[] = "index.html";

	ROOT = getenv("PWD");

	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) > 0)
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

	if(bind(sock_fd, (struct sockaddr *) &address, sizeof(address)) == 0)
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
		if(listen(sock_fd, 10) < 0)
		{
			perror("server: listen");
			exit(1);
		}

		if((clientfd = accept(sock_fd, (struct sockaddr *) &address, &addrlen)) < 0)
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

		// Split into tokens

		char ** tokens = malloc(100);
		int split = 0;
		char * token;
		token = strtok(buffer, " \n");
		while(split <= 2)
		{
			tokens[split] = token;
			token = strtok(NULL, " \n");
			split++;
		}

		// Find Request Method and File Path

		char * METHOD = tokens[0];
		char * PATH = tokens[1];

		// Add root path

		char final_path[1000] = "";
		strcpy(final_path, ROOT);
		strcat(final_path, PATH);

		// Append index.html if required

		if(final_path[strlen(final_path)-1] == '/')
		{
			strcat(final_path, index);
		}

		printf("File: %s\n", final_path);

		// Get Response Code
		int responseCode = get_response_code(METHOD, final_path);
		printf("%d\n", responseCode);

		// Send Response Code and Response Headers
		response(clientfd, responseCode);

		// Send Response Body if responseCode is 200 OK
		response_body(clientfd, final_path, responseCode);

		close(clientfd);
	}

	close(sock_fd);

	return 0;
}