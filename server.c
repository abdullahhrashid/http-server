#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  
#include <unistd.h> 
#include"httpResponse.h"

#define BUFFER_SIZE 4096
#define PORT 9090

int main(){

    int socket_fd=socket(AF_INET,SOCK_STREAM,0);
    char buffer [BUFFER_SIZE];

    if(socket_fd==-1){
        perror("Socket Creation Failed");
        return 1;
    }
    
    struct sockaddr_in socket_address;
    socket_address.sin_family=AF_INET;
    socket_address.sin_port=htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &socket_address.sin_addr); 
    //socket_address.sin_addr.s_addr=INADDR_ANY;

    if(bind(socket_fd,(struct sockaddr *) &socket_address, sizeof(socket_address))<0){
        perror("Couldn't bind Socket");
        close(socket_fd);
        return 1;
    };

    if(listen(socket_fd,5)<0){
        perror("Couldn't listen");
        close(socket_fd);
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    
    struct sockaddr_in client_address;
    socklen_t client_length=sizeof(client_address);
    int client_fd;
    
    while(1){

        client_fd=accept(socket_fd,(struct sockaddr *)&client_address,&client_length);

        if(client_fd<0){
            perror("Error in Accepting Client Request");
            continue; 
        }

        memset(buffer,0,BUFFER_SIZE-1);
        read(client_fd,buffer,BUFFER_SIZE);
        printf("Client Request:");
        printf("\n%s\n",buffer);

        char * response = (char*)malloc(sizeof(char)*BUFFER_SIZE);
        size_t response_length;
        char* file_name = "hello.html";

        http_response(file_name,response,&response_length);
        write(client_fd,response,response_length);

        close(client_fd);
    }

    close(socket_fd);
    return 0;
}