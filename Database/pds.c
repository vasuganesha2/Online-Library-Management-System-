#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "pds.h"

//struct PDS_RepoInfo repo_handle;
struct PDS_Repo repo_handle1;
struct PDS_Repo repo_handle2;
struct PDS_Repo repo_handle3;
//BOOK AND USER KA DATABASE BANA DUNGA...



int acquire_lock(FILE *fp) {
    int fd = fileno(fp);
    if (flock(fd, LOCK_EX) == -1) {
        perror("Failed to acquire lock");
        return PDS_FILE_ERROR;
    }
    return PDS_SUCCESS;
}

// Function to release the lock on the data file
int release_lock(FILE *fp) {
    int fd = fileno(fp);
    if (flock(fd, LOCK_UN) == -1) {
        perror("Failed to release lock");
        return PDS_FILE_ERROR;
    }
    return PDS_SUCCESS;
}



int pds_create(char *repo_name)
{
	char file[40];
    char index[40];
	strcpy(file,repo_name);
	strcat(file,".dat");
	strcpy(index,repo_name);
	strcat(index,".ndx");
	FILE *fp1 = fopen(file,"wb+");
	FILE *fp2 = fopen(index,"wb+");
	//Making an Index file and a data file....
	if(fp1==NULL || fp2==NULL)
	{
		return PDS_FILE_ERROR;
	}
	int l = 0;
	fwrite(&l,sizeof(int),1,fp2);
	fclose(fp1);
	fclose(fp2);
	return PDS_SUCCESS;
}

//OPEN KAR DUNGA DONO FILE ISSIS FUNCTION KI HELP SA...
int pds_open( char *repo_name, int rec_size, struct PDS_Repo *repo)
{
	char file[35], index[35];

	strcpy(file,repo_name);

	strcat(file,".dat");

	strcpy(index,repo_name);

	strcat(index,".ndx");

	repo -> pds_data_fp = fopen(file,"rw+");   

	repo -> pds_ndx_fp = fopen(index,"rw+");

    //Opening a .dat file and an .ndx file  Opening both in read back form....

	if(repo -> pds_data_fp == NULL || repo -> pds_ndx_fp == NULL) return PDS_FILE_ERROR;
 
	repo -> repo_status = PDS_REPO_OPEN;

	repo -> rec_size = rec_size;

	strcpy(repo -> pds_name, repo_name);

	pds_load_ndx(repo);

	fclose(repo -> pds_ndx_fp);

	return PDS_SUCCESS;
}

int pds_open2(char *repo_name, struct PDS_Repo *repo)
{
	char file[50];
	strcpy(file,repo_name);
	strcat(file,".dat");

	repo -> pds_data_fp = fopen(file,"rw+");   
	return PDS_SUCCESS;
}


//DONO KA LIYA KAAM KARDENGA...

int pds_load_ndx(struct PDS_Repo *repo) 
{
    struct PDS_NdxInfo var;
    FILE *fp = repo -> pds_ndx_fp;
    while (fread(&var, sizeof(struct PDS_NdxInfo), 1, fp) == 1) 
	{
        bst_add_node(&(repo -> ndx_root), var.key, &var);
		repo -> rec_count++;
    }
    return PDS_SUCCESS;
}

//DONO KA LIYA KAAM KARDENGA....

int put_rec_by_key( int key, void *rec, struct PDS_Repo *repo )
{
	if(acquire_lock(repo ->pds_data_fp) != PDS_SUCCESS) return PDS_ADD_FAILED;
	if(repo -> repo_status == PDS_REPO_CLOSED) 
    {
        return PDS_ADD_FAILED;
    }

	struct PDS_NdxInfo *entity = malloc(sizeof(struct PDS_NdxInfo));

	entity->key = key;
	fseek(repo -> pds_data_fp,0,SEEK_END);

	entity->offset = ftell(repo -> pds_data_fp);

	int k = bst_add_node(&(repo -> ndx_root),key,entity);

    if(k)
	{
		return PDS_FILE_ERROR;
	}

	fwrite(rec,repo -> rec_size,1,repo -> pds_data_fp);
	repo -> rec_count++;
	release_lock(repo -> pds_data_fp);
	return PDS_SUCCESS;
}

//DONO KA LIYA KAAM KARDENGA...

int get_rec_by_key( int key, void *rec , struct PDS_Repo *repo)
{
	if(acquire_lock(repo ->pds_data_fp) != PDS_SUCCESS) return PDS_ADD_FAILED;
	struct BST_Node *node = bst_search(repo -> ndx_root, key);
	if(!node)
	{
		
		return PDS_REC_NOT_FOUND;
	}
	struct PDS_NdxInfo *entity = (struct PDS_NdxInfo*)node -> data;
	fseek(repo -> pds_data_fp, entity -> offset, SEEK_SET);
	fread(rec,repo -> rec_size, 1, repo -> pds_data_fp);
	release_lock(repo -> pds_data_fp);
	return PDS_SUCCESS; 
}

int update_book_number_by_key(int key, void * rec, struct PDS_Repo *repo)
{
	if(acquire_lock(repo ->pds_data_fp) != PDS_SUCCESS) return PDS_ADD_FAILED;
	struct BST_Node *node = bst_search(repo -> ndx_root, key);
	if(node == NULL)
	{
	
		return PDS_REC_NOT_FOUND;
	}

	struct PDS_NdxInfo *entity = (struct PDS_NdxInfo*)node -> data;
	fseek(repo -> pds_data_fp, entity -> offset, SEEK_SET);
	fwrite(rec,repo -> rec_size, 1, repo -> pds_data_fp);

	release_lock(repo -> pds_data_fp);
	return PDS_SUCCESS; 
}



//DONO KA LIYA KAAM KARDENGA...
void bst_entry_into_file(struct BST_Node *root, struct PDS_Repo *repo)
{
	if (!root) return;
	fwrite(root -> data, sizeof(struct PDS_NdxInfo), 1, repo -> pds_ndx_fp);
	bst_entry_into_file(root -> left_child, repo);
	bst_entry_into_file(root -> right_child, repo);
}

int update_user_by_key(int key, void * rec, struct PDS_Repo *repo)
{
	if(acquire_lock(repo ->pds_data_fp) != PDS_SUCCESS) return PDS_ADD_FAILED;

	struct BST_Node *node = bst_search(repo -> ndx_root, key);
	if(!node)
	{
		
		return PDS_REC_NOT_FOUND;
	}
	struct PDS_NdxInfo *entity = (struct PDS_NdxInfo*)node -> data;
	fseek(repo -> pds_data_fp, entity -> offset, SEEK_SET);
	fwrite(rec,repo -> rec_size, 1, repo -> pds_data_fp);

	release_lock(repo -> pds_data_fp);

	return PDS_SUCCESS; 
}

int put_rec(long long book_id,long long user_id, struct PDS_Repo *repo)
{
	if(acquire_lock(repo ->pds_data_fp) != PDS_SUCCESS) return PDS_ADD_FAILED;
	if(repo -> pds_data_fp == NULL)
	{
		return PDS_ADD_FAILED;
	}
	fseek(repo -> pds_data_fp, 0, SEEK_END);

	fwrite(&user_id,sizeof(long long), 1, repo -> pds_data_fp);

	fwrite(&book_id, sizeof(long long), 1 ,repo -> pds_data_fp);
	release_lock(repo -> pds_data_fp);
	return PDS_SUCCESS;
}


int del_rec(long long  book_id, long long user_id, struct PDS_Repo *repo) 
{
	if(acquire_lock(repo ->pds_data_fp) != PDS_SUCCESS) return PDS_ADD_FAILED;
    fseek(repo->pds_data_fp, 0, SEEK_SET); // Move file pointer to the beginning
    
    long long  a, b;
    int record_deleted = 0; // Flag to indicate if the record is deleted
    
    while (fread(&a, sizeof(long long), 1, repo->pds_data_fp)) {
        fread(&b, sizeof(long long), 1, repo->pds_data_fp);
        if (a == user_id && b == book_id) {
            fseek(repo->pds_data_fp, -2 * sizeof(long long), SEEK_CUR);
            a = -1; 
            b = -1;
            fwrite(&a, sizeof(long long), 1, repo->pds_data_fp);
            fwrite(&b, sizeof(long long), 1, repo->pds_data_fp);
            record_deleted = 1;
			return PDS_SUCCESS;
        }
    }
	release_lock(repo -> pds_data_fp);
    return PDS_ADD_FAILED;
}



int pds_close(struct PDS_Repo *repo)
{
	char index[40];
	strcpy(index, repo -> pds_name);
	strcat(index, ".ndx");
	FILE* temp = fopen(index, "wb");
	fwrite(&(repo -> rec_count), sizeof(int), 1, temp);
	bst_entry_into_file(repo -> ndx_root, repo);
	fclose(temp); 
	fclose(repo -> pds_data_fp);
	fclose(repo -> pds_ndx_fp);
	return PDS_SUCCESS;
}

int pds_close2(struct PDS_Repo *repo)
{
	fclose(repo -> pds_data_fp);
	return PDS_SUCCESS;
}