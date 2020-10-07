#include "../include/http-server.h"

void ParseHttpHeaderAndCreateResponse(char *header, char *Response)
{
  char first_line_of_request[SIZE];
  HttpHeader hh;
  char *save_ptr = NULL;
  FILE *fp = NULL;
  char file_contents[8192];

  if(header == NULL) {
    strcpy(Response, BAD_REQUEST);
    return; 
  }
  int i = 0;
  while(header[i] != '\n') {
    first_line_of_request[i] = header[i];
    i++;
  }
  puts(first_line_of_request);

  hh.type = strtok_r(first_line_of_request, " ", &save_ptr);
  hh.filename = strtok_r(save_ptr, " ", &save_ptr);
  hh.connection = strtok_r(save_ptr, " ", &save_ptr);

  printf("Header content\ntype->%s\nfilename->%s\nconnection->%s\n", 
          hh.type, hh.filename, hh.connection);

  if((access(hh.filename, F_OK)) < 0) {
    strcpy(Response, FILE_NOT_FOUND);
    return; 
  }
  
  if(strcmp(hh.filename, "/") == 0) {
    strcpy(file_contents, "Hello, I'm a Simple Http Server :D");
    sprintf(Response, "HTTP/1.1 200 OK\nContent-Type: text/plain\n \
            Content-Length: %ld\n\n%s", strlen(file_contents), file_contents);
  }

  int ch = 0;
  i = 0;
  if((fp = fopen(hh.filename, "r")) != NULL) {
    while((ch = fgetc(fp)) != EOF) {
      file_contents[i] = ch;
      i++;
    }
  }

  sprintf(Response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: \
      %ld\n\n%s", strlen(file_contents), file_contents);

  printf("--------------Response-------------\n");
  puts(Response);
  printf("--------------Response-------------\n");

}

int main(int argc, char **argv)
{
  struct sockaddr_in server;
  int sockfd;
  int status;
  int clientfd;
  char buf[SIZE];
  char Response[65535];
  int opt = 1;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0)
    err_exit("socket");

  server.sin_family = AF_INET;
  server.sin_port = htons(10000);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                   &opt, sizeof(opt));
  if(status < 0)
    err_exit("setsockopt")

  status = bind(sockfd, (struct sockaddr *)&server, sizeof(server));
  if(status < 0) 
    err_exit("bind");

  status = listen(sockfd, BACKLOG);
  if(status < 0) 
    err_exit("listen"); 
  
  while(1) {
    printf("Waiting for connection...\n");
    clientfd = accept(sockfd, NULL, NULL);
    read(clientfd, buf, SIZE);
    puts(buf);
    ParseHttpHeaderAndCreateResponse(buf, Response);
    bzero(buf, SIZE);
    write(clientfd, Response, strlen(Response));
    close(clientfd);
  }
  close(sockfd);
  return 0;
}
