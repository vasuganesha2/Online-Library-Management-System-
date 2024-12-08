#include <CUnit/Basic.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Database/User.h"
#include "Database/pds.h"
#include "Database/bst.h"

// Mock hash function for testing purposes
long long mock_hash(char *str) {
    long long hash = 0;
    int c;

    while ((c = *str++)) {
        hash = hash * 10 + ((c - 'a')) % 10 + 1;
    }
    return hash;
}

// Test for User Registration
void test_add_user() {
    struct User user;
    user.user_id = mock_hash("testuser");
    strcpy(user.name, "testuser");
    strcpy(user.password, "testpassword");

    int status = add_user(&user);
    CU_ASSERT_EQUAL(status, USER_SUCCESS);
}

// Test for User Login
void test_login_user() {
    struct User user;
    user.user_id = mock_hash("testuser");
    strcpy(user.name, "testuser");
    strcpy(user.password, "testpassword");

    int status = search_user(user.user_id, &user, user.password);
    CU_ASSERT_EQUAL(status, USER_SUCCESS);
}

// Test for Adding a Book
void test_add_book() {
    struct Book book;
    book.book_id = mock_hash("testbook");
    strcpy(book.title, "testbook");
    strcpy(book.author, "testauthor");
    book.copies = 5;

    int status = add_new_book(book.book_id, &book);
    CU_ASSERT_EQUAL(status, PDS_SUCCESS);
}

// Test for Borrowing a Book
void test_borrow_book() {
    struct Book *book = malloc(sizeof(struct Book));
    if (!book) {
        CU_FAIL("Memory allocation for book failed");
        return;
    }

    int user_id = mock_hash("testuser");

    if (get_rec_by_key(mock_hash("testbook"), book, &repo_handle2) == PDS_SUCCESS) {
        printf("Book fetched: %s, Copies before decrement: %d\n", book->title, book->copies);

        int status = decrement_book(mock_hash("testbook"), user_id, book);
        CU_ASSERT_EQUAL(status, PDS_SUCCESS);
    } else {
        CU_FAIL("Failed to fetch book record.");
    }

    free(book); // Free memory safely
    book = NULL; // Prevent dangling pointer
}

// Test for Returning a Book
void test_return_book() {
    struct Book book;
    int user_id = mock_hash("testuser");

    if (get_rec_by_key(mock_hash("testbook"), &book, &repo_handle2) == PDS_SUCCESS) {
        int status = increment_book(mock_hash("testbook"), user_id, &book);
        CU_ASSERT_EQUAL(status, PDS_SUCCESS);
    } else {
        CU_FAIL("Failed to fetch book record.");
    }
}

// Initialization Suite
int init_suite(void) {
    if (pds_create("test_user_repo") != PDS_SUCCESS) {
        return -1;
    }
    if (pds_open("test_user_repo", sizeof(struct User), &repo_handle1) != PDS_SUCCESS) {
        return -1;
    }

    if (pds_create("test_book_repo") != PDS_SUCCESS) {
        return -1;
    }
    if (pds_open("test_book_repo", sizeof(struct Book), &repo_handle2) != PDS_SUCCESS) {
        return -1;
    }

    if (pds_create("test_borrow_repo") != PDS_SUCCESS) {
        return -1;
    }
    if (pds_open2("test_borrow_repo", &repo_handle3) != PDS_SUCCESS) {
        return -1;
    }

    return 0;
}

// Cleanup Suite
int clean_suite(void) {
    if (repo_handle1.pds_data_fp) {
        pds_close(&repo_handle1);
        repo_handle1.pds_data_fp = NULL;
    }

    if (repo_handle2.pds_data_fp) {
        pds_close(&repo_handle2);
        repo_handle2.pds_data_fp = NULL;
    }

    if (repo_handle3.pds_data_fp) {
        pds_close(&repo_handle3);
        repo_handle3.pds_data_fp = NULL;
    }

    return 0;
}

// Main function to run the tests
int main() {
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("User and Book Tests", init_suite, clean_suite);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(suite, "Test User Registration", test_add_user)) ||
        (NULL == CU_add_test(suite, "Test User Login", test_login_user)) ||
        (NULL == CU_add_test(suite, "Test Add Book", test_add_book)) ||
        (NULL == CU_add_test(suite, "Test Borrow Book", test_borrow_book)) ||
        (NULL == CU_add_test(suite, "Test Return Book", test_return_book))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
