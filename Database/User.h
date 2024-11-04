#ifndef USER_H
#define USER_H

#define USER_SUCCESS 0
#define USER_FAILURE 1
 
struct Book
{
	long long  book_id;
	char author[30];
	char title[40];
	int copies;
};

struct User
{
	long long user_id;
	char name[30];
	char password[15];
	int books[50];
};



void print_user( struct User *c );
int add_user( struct User *c );
int search_user(int contact_id, struct User * c,char *password);
int add_more_copies(int book_id,struct Book *b,int num);
int add_new_book(int book_id,struct Book *b);
int increment_book(int book_id,int user_id, struct  Book *b);
int decrement_book(int book_id,int user_id,struct  Book *b);
int get_books();
int show_rec(int user_id);

#endif