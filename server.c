#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> // read(), write(), close()
#include <pthread.h>
#include "./Database/User.h"
#include "./Database/pds.h"
#include <openssl/sha.h>
#include <signal.h>
// struct PDS_Repo repo_handle1;
// struct PDS_Repo repo_handle2;
// struct PDS_Repo repo_handle3;

#define MAX 80 
#define PORT 7222
#define SA struct sockaddr 


long long hash(char *str) {
    long long hash = 0;
    int c;

    while ((c = *str++)) {
        hash = hash * 10 + ((c - 'a')) % 10 + 1;
	
    }
    return hash;
}

void func1()
{
    pds_close(&repo_handle1);
	pds_close(&repo_handle2);
	
    exit(1);
}

// Function designed for chat between client and server. 
void *func(void *p_connfd) 
{ 
    int connfd = *((int *)(p_connfd));
    free(p_connfd);
	char buff[MAX]; 
	char rec[MAX];
	int n; 
	//signal(SIGINT | SIGTERM, func1);
	for (;;) 
    { 
		bzero(buff, MAX); 
		bzero(rec,MAX);

		read(connfd, buff, sizeof(buff)); 
		
		
		if (strncmp("exit", buff,4) == 0) 
		{ 
			printf("Connection Exit...\n"); 
			
			close(connfd);
    		pthread_exit(NULL);
			break; 
		} 

		char* token = strtok(buff, ":");

		int num_tokens = 0;
		char* tokens[8];

		while (token != NULL) 
		{
			tokens[num_tokens] = token;
		
			token = strtok(NULL, ":");
		
			num_tokens++;
		}
		

		if(strncmp(tokens[0],"auth",strlen("auth")) == 0)
		{
			if(strncmp(tokens[1],"store",strlen("store")) == 0)
			{

				struct User user;
				user.user_id = hash(tokens[2]);
				strcpy(user.name, tokens[2]);
				strcpy(user.password, tokens[3]);

				int status = add_user(&user);
		

				if(status == USER_SUCCESS)
				{

					sprintf(rec, "%lld", user.user_id);
					write(connfd, rec, sizeof(rec));
					printf("NEW USER ADDED \n");

				}
				else
				{

					printf("FAILED \n");
					write(connfd, "NO", sizeof("NO"));
				}
			}

			if(strncmp(tokens[1],"login",sizeof("login")) == 0)
			{
				struct User user;
				user.user_id = hash(tokens[2]);
				strcpy(user.name, tokens[2]);
				strcpy(user.password, tokens[3]);
				int status = search_user(user.user_id, &user, user.password);
				printf("%s %s %lld \n",tokens[2],tokens[3],user.user_id);
				if(status == USER_SUCCESS)
				{
					sprintf(rec, "%lld", user.user_id);
					write(connfd, rec, sizeof(rec));
					printf("Login Succesfully\n");
				}
				else
				{
					write(connfd, "NO", sizeof("NO"));
					printf("Login Failed\n");
				}
			}
		}

		if(strncmp(tokens[0],"trans",sizeof("trans")) == 0)
		{
			if(strncmp(tokens[1],"inc",sizeof("inc")) == 0)
			{
				//Returning the book....
				struct Book book;
				int user_id;
				user_id = atoi(tokens[3]);
		
				if(increment_book(hash(tokens[2]), user_id, &book) == PDS_SUCCESS)
				{	
					write(connfd, "Book removed succesfully", sizeof(rec));
				}
				else
				{
					write(connfd, "Failed", sizeof(rec));
				}
			}

			//printf("%d hi \n", strncmp(tokens[1],"dec",sizeof("dec")));
			if(strncmp(tokens[1],"dec",sizeof("dec")) == 0)
			{
				struct Book book;
				int user_id;
				user_id = atoi(tokens[3]);
				
				if(decrement_book(hash(tokens[2]),user_id,&book) == PDS_SUCCESS)
				{
					write(connfd,"Book added successfully",sizeof(rec));
				}	
				else
				{
					write(connfd,"Failed to add book",sizeof("Failed to add book"));
				}
			}
			
			if(strncmp(tokens[1], "add",sizeof("add")) == 0)
			{
				struct Book* book = malloc(repo_handle2.rec_size);
				struct Book* book1 = malloc(repo_handle2.rec_size);
				book -> book_id = hash(tokens[2]);
				strcpy(book -> title,tokens[2]);
				strcpy(book -> author,tokens[3]);
				int copies = atoi(tokens[4]);

				// Store the integer value into book->copies
				book->copies = copies;
				if(get_rec_by_key(hash(tokens[2]),book1, &repo_handle2) != PDS_SUCCESS)
				{
					if(add_new_book(book -> book_id, book) == PDS_SUCCESS)
					{
						write(connfd,"Book added successfully",100);
					}
					else
					{
						write(connfd,"Book unable to add",100);
					}
				}
			}

			if(strncmp(tokens[1], "modify",sizeof("modify")) == 0)
			{

				int book_id = hash(tokens[2]);

				int num = atoi(tokens[3]);



				struct Book *book = malloc(repo_handle2.rec_size);
				if(get_rec_by_key(book_id,book, &repo_handle2) == PDS_SUCCESS)
				{
					book -> copies = num;
					if(add_more_copies(book_id, book, num) == PDS_SUCCESS)
					{
						write(connfd,"Book modify successfully",200);
					}
					else
					{
						write(connfd,"BOOK NOT MODIFY !!",100);
					}
				}
				else
				{
					write(connfd,"BOOK NOT MODIFY !!",100);
				}
			}
			if(strncmp(tokens[1],"check",sizeof("check"))  == 0)
			{
				if(get_books() == PDS_SUCCESS)
				{
					write(connfd,"BOOKS",100);
				}
				else write(connfd,"FAILED",100);
			}

			if(strncmp(tokens[1],"show",sizeof("show")) == 0)
			{
				int user_id = atoi(tokens[2]);

				if(show_rec(user_id) ==PDS_SUCCESS)
				{
					write(connfd,"DATA OF USER",100);
				}
				else
				{
					write(connfd,"Error Fetching Data",100);	
				}
			}
		}

		bzero(buff, MAX); 
	} 
    close(connfd);
    pthread_exit(NULL);
    return NULL;
} 

// Driver function 
int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	signal(SIGINT,func1);
	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 
	int tr = 1;
	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&tr,sizeof(int)) == -1) 
	{
		perror("setsockopt");
		exit(1);
	}
	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 50)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 

    pthread_t tid[60];
    int i = 0;
	int rec_size1 = sizeof(struct User);
	int status = pds_create("info");
	if(status != PDS_SUCCESS)
	{

		printf("ERROR \n");
		exit(1);	
	}

	if(pds_open("info", rec_size1, &repo_handle1) != PDS_SUCCESS)
	{
		printf("ERROR \n");
		exit(1);
	}

	if(pds_create("library") != PDS_SUCCESS)
	{
		printf("ERROR \n");
		exit(1);	
	}
	int rec_size2 = sizeof(struct Book);
	if(pds_open("library" ,rec_size2, &repo_handle2) != PDS_SUCCESS)
	{
		printf("ERROR \n");
		exit(1);	
	}

	if(pds_create("linked") != PDS_SUCCESS)
	{
		printf("ERROR \n");
		exit(1);	
	}

	if(pds_open2("linked",&repo_handle3) != PDS_SUCCESS)
	{
		printf("ERROR \n");
		exit(1);		
	}

	FILE *fp = fopen("admin.txt", "r");
	if(fp == NULL)
	{
		printf("ERROR \n");
		exit(1);
	}

	char word[30];


    while (fscanf(fp, "%s", word) != EOF) 
	{
		struct User user;
		user.user_id = hash(word);
		strcpy(user.name , word);
		fscanf(fp, "%s", word);
		strcpy(user.password , word);
		add_user(&user);
    }
	// Accept the data packet from client and verification 
    while(1)
    {
	    len = sizeof(cli); 
        connfd = accept(sockfd, (SA*)&cli, &len); 
        if (connfd < 0) 
        { 
            printf("server accept failed...\n"); 
            exit(0); 
        } 
        else printf("server accept the client...\n"); 

        int * p_client = malloc(sizeof(int));
        *(p_client) = connfd;   
        //pthread_t t;                  //This Process will work Indiviually...                     
        pthread_create(&tid[i],NULL,func,p_client);
		//pthread_join(&tid[i], NULL);
		i++;
    }

	for (int j = 0; j < i; j++) 
	{
    	pthread_join(tid[j], NULL);
	}
    close(sockfd); 
}
