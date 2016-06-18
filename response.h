/**
 *  response.h
 */

// Response Codes Value
#define OK 0
#define NOT_FOUND 1
#define BAD_REQUEST 2

/**
 * Function that writes status code along with headers in the given socket file descriptor
 */
void response(int sock_fd, int status_code);

/**
 * Function that returns Status Code for given HTTP Request
 */
int get_response_code(char * method, char * path);

/**
 * Function that writes the response body to the given socket file descriptor
 */
void response_body(int sock_fd, char * path, int status_code);