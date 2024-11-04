#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h> // You'll typically need stdio.h for input/output functions
#include <stdlib.h> // You may need stdlib.h for memory allocation functions
#include <unistd.h> // You may need unistd.h for close() function
#include <string.h>
#include<signal.h>
#include "client.h"


#define pr(x) printf("%d \n",x)
#define BUFFER_SIZE 4096
#define PORT 7222

void handler() 
{
    printf("System Exit !!!! \n");
    
    exit(EXIT_SUCCESS);
}

char *process_data(char* rec,int* sockfd)
{

    if(strncmp("exit",rec,4) == 0) 
    {
        printf("System Exit !!!! \n");
        send(*(sockfd), "exit", 50, 0);
        close(*sockfd);
        exit(EXIT_SUCCESS);
    }

    int a = send (*(sockfd), rec, 50,0);
    
    if(a < 0)
    {
        perror("Issue in Sending the Data !!!!!");
        close(*sockfd);
        exit(EXIT_SUCCESS);
    }

    char *buffer = malloc(BUFFER_SIZE);
    int b = recv (*(sockfd), buffer, BUFFER_SIZE,0);
    if(b < 0) 
    {
        perror("Issue in fetching the Data!!!");
        close(*sockfd);
        exit(EXIT_SUCCESS);
    }
    //a = send (*(sockfd), "exit", 50,0);

    return buffer;
}



int *setup()
{
    signal(SIGINT, handler);
    int *p_sockfd = malloc(sizeof(int)); 
   *p_sockfd = socket (AF_INET, SOCK_STREAM, 0);
   //AF_INTENT6 TO USE IPV6 adresses system creating a addres for the client..
   //SOCK_STREAM for TCP Connection For UPD it is SOCK_DGRAM..
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    // inet_pton(AF_UNIX, "127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    //inet_pton is converting IPV4 or IPV6 adress into the presentation format..
    //The connection accepted by the server..
    //The port number of the server


    int con = connect(*(p_sockfd),(struct sockaddr *)&server_addr,sizeof(server_addr));
    //Connecting to the Server..
    if(con < 0)
    {
            printf("Connection Failure !!!!!! \n");
            exit(EXIT_FAILURE);
            //EXiting if the Client is unable to connect to the Server...
    }  
    //close(*p_sockfd);
    // exit(EXIT_SUCCESS);
    return p_sockfd;
}

































