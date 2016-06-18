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

// Status Codes
char * text_200 = "HTTP/1.1 200 OK\n";
char * text_404 = "HTTP/1.1 404 Not Found\n";
char * text_400 = "HTTP/1.1 400 Bad Request\n";

// Headers
char * content_type = "Content-Type: text/html; charset=ISO-8859-1\n";

/**
 * Function that writes status code along with headers in the given socket file descriptor
 */
void response(int sock_fd, int status_code)
{
	if(status_code == OK)
	{
		write(sock_fd, text_200, strlen(text_200));
		write(sock_fd, content_type, strlen(content_type));
		write(sock_fd, "\n", 1);
	}
	else if(status_code == NOT_FOUND)
	{
		write(sock_fd, text_404, strlen(text_404));
		write(sock_fd, "\n", 1);
	}
	else
	{
		write(sock_fd, text_400, strlen(text_400));
		write(sock_fd, "\n", 1);
	}
}

/**
 * Function that returns Status Code for given HTTP Request
 */
int get_response_code(char * method, char * path)
{
	if(!(strcasecmp(method, "GET") == 0 || strcasecmp(method, "HEAD") == 0))
	{
		return BAD_REQUEST;
	}
	int fd = open(path, O_RDONLY);
	if(fd == -1)
	{
		return NOT_FOUND;
	}
	close(fd);
	return OK;
}

/**
 * Function that writes the response body to the given socket file descriptor
 */
void response_body(int sock_fd, char * path, int status_code)
{
	if(status_code == OK)
	{
		int fd = open(path, O_RDONLY);
		int bytes_read;

		if(fd == -1)
		{
			perror("server: file");
			exit(1);
		}

		char * buffer = malloc(1024);
		while((bytes_read = read(fd, buffer, 1024)) > 0)
		{
	    	write(sock_fd, buffer, bytes_read);
		}
	}
	else if(status_code == NOT_FOUND)
	{
		write(sock_fd, "<h1>404 Not Found!</h1>\n", 25);
	}
	else
	{
		write(sock_fd, "<h1>400 Bad Request!</h1>\n", 27);
	}

	return;
}