#ifndef HTTP_SERVER_HEADER
#define HTTP_SERVER_HEADER

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define err_exit(msg) { \
                      perror(msg); \
                      exit(-1); \
                      }

#define BACKLOG 10
#define SIZE 1024
#define BAD_REQUEST "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\n \
                      Content-Length: 16\n\n400 Bad Request!"
#define FILE_NOT_FOUND "HTTP/1.1 404 File Not Found\nContent-Type: \
                      text/plain\nContent-Length: 19\n\n404 File Not Found!"

// HttpHeader structure will store first line of the request.

typedef struct HttpHeader {
  char  *type;
  char  *filename;
  char  *connection;
}HttpHeader;


void ParseHttpHeaderAndConstructResponse(char *header, char *Response);

#endif 
