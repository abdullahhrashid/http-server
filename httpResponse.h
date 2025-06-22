#define BUFFER_SIZE 4096
#include<stdlib.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

void http_response(char * file_name, char * response, size_t* response_length){

    int file_fd = open(file_name,O_RDONLY);
    if(file_fd==-1){
        snprintf(response,BUFFER_SIZE,"HTTP/1.1 404 Not Found\r\n"
                                      "Content-Type: text/html\r\n"
                                      "\r\n"
                                      "<h1 style=\"color:#b32d2e\">"
                                      "Error 404</h1>"
                                      "<h2>Page Not Found</h2>");
        *response_length=strlen(response);
        return;
    }

    char* header = (char*) malloc(BUFFER_SIZE*sizeof(char));
    snprintf(header,BUFFER_SIZE,"HTTP/1.1 200 OK\r\n"
                                "Content-Type: text/html\r\n"
                                "\r\n");
    *response_length=0;
    memcpy(response,header,strlen(header));
    *response_length+=strlen(header);

    ssize_t byte_reads;

    while((byte_reads=read(file_fd,response+*response_length,BUFFER_SIZE-*response_length))>0){
        *response_length+=byte_reads;
    }

    free(header);
    close(file_fd);
    return;
}