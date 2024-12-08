#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 7222
#define SA struct sockaddr

void test_server(int sockfd) 
{
    char buff[MAX];
    char response[MAX];
    int n;

    // Testing User Registration
    printf("Testing User Registration...\n");
    snprintf(buff, sizeof(buff), "auth:store:testuser:testpassword");
    write(sockfd, buff, strlen(buff));
    bzero(response, sizeof(response));
    read(sockfd, response, sizeof(response));
    printf("Server Response: %s\n\n", response);

    // Testing User Login
    printf("Testing User Login...\n");
    snprintf(buff, sizeof(buff), "auth:login:testuser:testpassword");
    write(sockfd, buff, strlen(buff));
    bzero(response, sizeof(response));
    read(sockfd, response, sizeof(response));
    printf("Server Response: %s\n\n", response);

    // Testing Add Book
    printf("Testing Add Book...\n");
    snprintf(buff, sizeof(buff), "trans:add:booktitle:bookauthor:5");
    write(sockfd, buff, strlen(buff));
    bzero(response, sizeof(response));
    read(sockfd, response, sizeof(response));
    printf("Server Response: %s\n\n", response);

    // Testing Borrow Book
    printf("Testing Borrow Book...\n");
    snprintf(buff, sizeof(buff), "trans:dec:booktitle:123");
    write(sockfd, buff, strlen(buff));
    bzero(response, sizeof(response));
    read(sockfd, response, sizeof(response));
    printf("Server Response: %s\n\n", response);

    // Testing Return Book
    printf("Testing Return Book...\n");
    snprintf(buff, sizeof(buff), "trans:inc:booktitle:123");
    write(sockfd, buff, strlen(buff));
    bzero(response, sizeof(response));
    read(sockfd, response, sizeof(response));
    printf("Server Response: %s\n\n", response);

    // Testing Show Records
    printf("Testing Show Records...\n");
    snprintf(buff, sizeof(buff), "trans:show:123");
    write(sockfd, buff, strlen(buff));
    bzero(response, sizeof(response));
    read(sockfd, response, sizeof(response));
    printf("Server Response: %s\n\n", response);

    // Exit Command
    printf("Sending Exit Command...\n");
    snprintf(buff, sizeof(buff), "exit");
    write(sockfd, buff, strlen(buff));
    printf("Exiting...\n");
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    // Socket creation and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    } else {
        printf("Socket successfully created...\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    // Assign IP and port
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost
    servaddr.sin_port = htons(PORT);

    // Connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection to the server failed...\n");
        exit(0);
    } else {
        printf("Connected to the server...\n");
    }

    // Run the tests
    test_server(sockfd);

    // Close the socket
    close(sockfd);
    return 0;
}
