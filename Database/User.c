#include<stdio.h>
#include<stdlib.h>

#include "pds.h"

#include "User.h"

// struct PDS_Repo repo_handle1;
// struct PDS_Repo repo_handle2;

int add_user( struct User *c )
{
	int status;

	status = put_rec_by_key( c->user_id, c, &repo_handle1);

	if( status != PDS_SUCCESS )
    {
		//fprintf(stderr, "Unable to add contact with key %lld. Error %d", c->user_id, status );
		return USER_FAILURE;
	}
	return USER_SUCCESS;
}

void print_user( struct User *c )
{
	printf("%lld,%s,%s\n", c->user_id,c->name,c->password);
}

// Store the rec in a struct contact pointer
// Store the key in a char pointer
// Compare the phone values in key and record
// Return 0,1,>1 based on above condition
int match_password( void *rec, void *key )
{

	struct User *ptr = (struct User*)rec;
	char *temp = (char *)key;
	char *ptr2 = ptr -> password;

	if(strcmp(temp,ptr2) == 0)
	{
		//printf("hi1 \n");
		return 0;
	}
	return 1;
}

int search_user( int contact_id, struct User *c,char * password)
{
	if(get_rec_by_key( contact_id, c, &repo_handle1) == PDS_SUCCESS)
    {
        if(match_password(c,password) == 0) return USER_SUCCESS;
    }
    return USER_FAILURE;
}

int decrement_book(int book_id,int user_id, struct  Book *b)
{

	if(get_rec_by_key(book_id,b, &repo_handle2) == PDS_SUCCESS)
	{

		if(b -> copies == 0) return PDS_ADD_FAILED;
		b -> copies = b -> copies - 1;
		if(update_book_number_by_key(book_id, b, &repo_handle2) == PDS_SUCCESS)
		{
	
			if(put_rec(book_id,user_id,&repo_handle3) ==PDS_SUCCESS)
			{
				return PDS_SUCCESS; 
			}	
		}
	}
	return PDS_ADD_FAILED;
}

int increment_book(int book_id, int user_id, struct Book *b)
{
	if(get_rec_by_key(book_id,b, &repo_handle2) == PDS_SUCCESS)
	{
		b -> copies = b -> copies + 1;
		if(update_book_number_by_key(book_id, b, &repo_handle2) == PDS_SUCCESS)
		{
		
			if(del_rec(book_id,user_id,&repo_handle3) ==PDS_SUCCESS)
			{
				return PDS_SUCCESS; 
			}	
		}

	}
	return PDS_ADD_FAILED;
}


int show_rec(int user_id)
{
	struct PDS_Repo* repo = &(repo_handle3);
	fseek(repo->pds_data_fp, 0, SEEK_SET); // Move file pointer to the beginning
    
    long long  a, b;
    int record_deleted = 0; // Flag to indicate if the record is deleted
    
    while (fread(&a, sizeof(long long), 1, repo->pds_data_fp)) {
        fread(&b, sizeof(long long), 1, repo->pds_data_fp);
        if (a == user_id) 
		{
			struct Book *book;
			get_rec_by_key(b, book,&repo_handle2);
			printf("%s %s\n",book -> title,book -> author);
        }
    }
	return PDS_SUCCESS;
}

int add_new_book(int book_id,struct Book *b)
{
	if(put_rec_by_key(book_id, b, &repo_handle2) == PDS_SUCCESS)
	{
		struct Book* b1 = malloc(repo_handle2.rec_size); 
		get_rec_by_key(book_id, b1,&repo_handle2);
		//printf("%s %s hi\n",b1 -> title, b1 -> author);
		return PDS_SUCCESS;
	}
	return PDS_ADD_FAILED;
}

int add_more_copies(int book_id,struct Book *b,int num)
{
	
	if(update_book_number_by_key(book_id, b, &repo_handle2) == PDS_SUCCESS) return PDS_SUCCESS; 
	return PDS_ADD_FAILED;
}

int get_books()
{
    FILE *fp = repo_handle2.pds_data_fp;
    if (fp == NULL) {
        return PDS_REC_NOT_FOUND;
    }
    
    fseek(fp, 0, SEEK_SET); // Move to the beginning of the file
    
    // Allocate memory for a book record
    struct Book *book = malloc(sizeof(struct Book));
    if (book == NULL) {
        // Handle memory allocation failure
        return PDS_FILE_ERROR;
    }

    // Loop through the file, reading book records
    while (fread(book, repo_handle2.rec_size, 1, fp) == 1)
    {
        // Process the book record
        printf("%s, %s, %d\n", book->title, book->author, book->copies);
    }

    // Free the memory allocated for the book record
    free(book);

    return PDS_SUCCESS;
}
