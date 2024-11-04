#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h> // You'll typically need stdio.h for input/output functions
#include <stdlib.h> // You may need stdlib.h for memory allocation functions
#include <unistd.h> // You may need unistd.h for close() function
#include <string.h>
#include "client.h"
#include <signal.h>

#define BUFFER_SIZE 4096

int* sockfd;

void func()
{
    process_data("exit", sockfd);
    
    exit(1);
}

__int32_t main()
{


    //SIGNAL HANDLING
    printf("|||||||||||||||||||||Welcome to Online Library of IIIT Banglore. |||||||||||||||||||||||| \n");
    int *p_sockfd = malloc(sizeof(int)); 
    p_sockfd = setup();
    sockfd = p_sockfd;
    signal(SIGINT, func);
    printf("Normal User or Adminstartion User.\n Press one for Normal User.\n Press two for Admintrative User.\n 0 for Exit \n");
    int t;
    scanf("%d", &t);

    if(t == 0)
    {
        
        process_data("exit", p_sockfd);
        close(*p_sockfd);
        exit(1);
    }
    

    if(t == 1)
    {
        //NORMAL USER
        char rec[50];
        strcpy(rec,"NO");
        while(strcmp(rec,"NO") == 0)
        {
            printf("Create Account or Login as an User... \n");
            int t;
            printf("Press One For Create Acount And Two For Login... Zero for exit\n");
            scanf("%d", &t);
            if(t == 0)
            {
                process_data("exit", p_sockfd);
                exit(1);
            }
            if(t == 1)
            {
                //Creating Account.....
                char name[30], password[15];
                printf("Enter a Unique User Name... \n");
                scanf("%s", name);
                printf("Enter a Password... \n");
                scanf("%s", password);
                char buf[100];
                 bzero(buf,100);
                strcat(buf, "auth:");
                strcat(buf, "store:");
                strcat(buf, name);
                strcat(buf, ":");
                strcat(buf, password);
                
                char *ptr = process_data(buf, p_sockfd);
                strcpy(rec, ptr);
                //record created....
                printf("%s \n", rec);
            }
            if(t == 2)
            {
                //Login as an User...
                
                char name[30],password[15];
                printf("Enter Your User Name... \n");
                scanf("%s",name);
                printf("Enter Your Password... \n");
                scanf("%s",password);
                char buf[100];
                 bzero(buf,100);
                strcat(buf, "auth:");
                strcat(buf, "login:");
                strcat(buf,name);
                strcat(buf,":");
                strcat(buf,password);
                char *ptr = process_data(buf,p_sockfd);
                strcpy(rec,ptr);
            }
        }   
        
        //BOOKS KA SATH BACKCHODI KARNI PADEGI...
        while(1)
        {
            char inf[100];
            int t;
            printf("1> Press Zero for exit..\n2> Press One For Borrow Book...\n3> Press Two For Return Book...\n4> Press Three to see Books...");
            scanf("%d", &t);
            if(t == 0) func();
            if(t == 1)
            {
                char title[30];
                printf("Enter Book Title... \n");
                scanf("%s",title);
                char buf[100];
                bzero(buf,100);
                strcat(buf, "trans:");
                strcat(buf, "dec:");
                strcat(buf,title);
                strcat(buf, ":");
                strcat(buf, rec);
                printf("%s \n",buf);
                char *ptr = process_data(buf,p_sockfd);
                strcpy(inf,ptr);  
                printf("%s \n",inf); 
            }
            if(t == 2)
            {
                char title[30];
                printf("Enter Book Title... \n");
                scanf("%s",title);
                char buf[100];
                bzero(buf,100);
                strcat(buf, "trans:");
                strcat(buf, "inc:");
                strcat(buf,title);
                strcat(buf, ":");
                strcat(buf, rec);
                char *ptr = process_data(buf,p_sockfd);
                strcpy(inf,ptr); 
                printf("%s \n",inf);   
            }
            if(t == 3)
            {
                char buf[100];
                bzero(buf,100);
                strcat(buf, "trans:");
                strcat(buf, "show:");
                strcat(buf, rec);
                char *ptr = process_data(buf,p_sockfd);
                strcpy(inf,ptr); 
                printf("%s \n",inf);  
            }
        }

    }

    if(t == 2)
    {
        //ADMINSTRATIVE USER
        char rec[50];
        strcpy(rec,"NO");
        //printf("hi1 \n");
        while(strncmp(rec,"NO",2) == 0)
        {
            int t;
            printf("Zero for Exit One for Login \n");
            scanf("%d", &t);

            if(t == 0)
            {
                process_data("exit", p_sockfd);
                exit(1);
            }

            char name[30],password[15];
            printf("Enter Your User Name... \n");
            scanf("%s",name);
            printf("Enter Your Password... \n");
            scanf("%s",password);
            char buf[100];
            bzero(buf,100);
            strcat(buf, "auth:");
            strcat(buf, "login:");
            strcat(buf,name);
            strcat(buf,":");
            strcat(buf,password);
            char *ptr = process_data(buf,p_sockfd);
            strcpy(rec,ptr);      
        }

        //BOOKS KA SATH BACKCHODI KARNI PADEGI...

        while(1)
        {
            char inf[100];
            int t;
            printf("Press Zero for exit..\nPress One For Add New Book...\nTPress Two For Modify Book...\n press three for data \n");
            scanf("%d", &t);
            if(t == 0) func();
            if(t == 1)
            {
                char title[30],author[15],copies[3];
                printf("Enter Book Title... \n");
                scanf("%s",title);
                printf("Enter Book Author... \n");
                scanf("%s",author);
                printf("Number of copies.. \n");
                scanf("%s",copies);
                char buf[100];
                bzero(buf,100);
                strcat(buf, "trans:");
                strcat(buf, "add:");
                strcat(buf,title);
                strcat(buf,":");
                strcat(buf,author);
                strcat(buf, ":");
                strcat(buf,copies);
                char *ptr = process_data(buf,p_sockfd);
                strcpy(inf,ptr);   
                printf("%s \n", ptr);
            }
            if(t == 2)
            {
                char title[30],copies[3];
                printf("Enter Book Title... \n");
                scanf("%s",title);
                printf("Number of copies.. \n");
                scanf("%s",copies);
                char buf[100];
                 bzero(buf,100);
                strcat(buf, "trans:");
                strcat(buf, "modify:");
                strcat(buf,title);
                strcat(buf,":");
                strcat(buf,copies);
                char *ptr = process_data(buf,p_sockfd);
                strcpy(inf,ptr); 
                printf("%s \n",inf);  
            }
            if(t == 3)
            {
                char buf[100];
                bzero(buf,100);
                strcat(buf, "trans:");
                strcat(buf, "check:");
                char *ptr = process_data(buf,p_sockfd);
                strcpy(inf,ptr);   
                printf("%s \n",inf); 
            }
        
        }
    }

    exit(EXIT_SUCCESS);
}