# Online Library Management System

## Overview

The **Online Library Management System** is a project designed to manage library operations effectively. It features a server-client architecture, where the server handles requests and manages the database, and the client provides a user-friendly interface for interaction. The project utilizes a binary search tree (BST) for fast database access, ensuring efficient retrieval of records.

## Prerequisites

- GCC (GNU Compiler Collection)
- pthread library for multi-threading support

## Compilation

To compile the server and client, follow these steps:

1. **Compile the Server:**

   Open a terminal and run the following command to run the server:

   ```bash
   gcc -o server Database/User.c Database/pds.c Database/bst.c server.c -lpthread
   ./server
   
 2. **Compile the Client:**
    Open another terminal  and run the following command to run the client:
       ```bash
     gcc -o logic client.c logic.c
     ./logic

