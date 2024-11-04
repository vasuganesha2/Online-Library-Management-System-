#ifndef PDS_H
#define PDS_H

#include "bst.h"

// Error codes
#define PDS_SUCCESS 0
#define PDS_FILE_ERROR 1
#define PDS_ADD_FAILED 2
#define PDS_REC_NOT_FOUND 3
#define PDS_REPO_ALREADY_OPEN 12
#define PDS_NDX_SAVE_FAILED 13
#define PDS_REPO_NOT_OPEN 14

// Repository status values
#define PDS_REPO_OPEN 10
#define PDS_REPO_CLOSED 11

#define MAX_NDX_SIZE 1000

struct PDS_NdxInfo {
    int key;
    int offset;
};

struct PDS_Repo {
    char pds_name[30];
    FILE *pds_data_fp;
    FILE *pds_ndx_fp;
    int repo_status;
    int rec_size;
    int rec_count;
    struct BST_Node *ndx_root;
};


extern struct PDS_Repo repo_handle1;
extern struct PDS_Repo repo_handle2;
extern struct PDS_Repo repo_handle3;


int pds_create(char *repo_name);
int pds_open(char *repo_name, int rec_size, struct PDS_Repo *repo);
int pds_open2(char *repo_name, struct PDS_Repo *repo);
int pds_load_ndx(struct PDS_Repo *repo);
int put_rec_by_key(int key, void *rec, struct PDS_Repo *repo);
int get_rec_by_key(int key, void *rec, struct PDS_Repo *repo);
int update_book_number_by_key(int key, void *rec, struct PDS_Repo *repo);
int pds_close(struct PDS_Repo *repo);
int update_user_by_key(int key, void * rec, struct PDS_Repo *repo);
int del_rec(long long book_id, long long user_id, struct PDS_Repo *repo);
int put_rec(long long book_id,long long user_id, struct PDS_Repo *repo);

#endif
