# Online Library Management System

## Overview

The **Online Library Management System** is a project designed to manage library operations effectively. It features a server-client architecture, where the server handles requests and manages the database, and the client provides a user-friendly interface for interaction. The project utilizes a binary search tree (BST) for fast database access, ensuring efficient retrieval of records.

## Prerequisites

- GCC (GNU Compiler Collection)
- pthread library for multi-threading support

---

## Feature Engineering

### Efficient Data Management

- **Database Implementation**: The project employs a Binary Search Tree (BST) for fast and efficient database operations, enabling quick access and updates to user and book records.
- **Indexing**: Utilizes `.dat` and `.ndx` files for structured storage and indexing of data, ensuring optimized lookups and minimal disk I/O.

### Threading and Concurrency

- **Multithreading**: Incorporates threading for simultaneous processing of multiple client requests, improving responsiveness and scalability.
- **Synchronization**: Ensures safe concurrent access to shared resources, preventing data corruption.

### Hashing for Security

- **Password Management**: Implements hashing mechanisms to securely store and validate passwords, protecting user credentials from potential breaches.

### Layered Architecture

- **Modular Design**: Organized into distinct modules like `client`, `server`, and `logic`, adhering to a clean separation of concerns for better maintainability.
- **Custom Libraries**: Includes reusable headers and source files, promoting extensibility.

### Extensibility

- **Dynamic Data Updates**: Designed to support new features like enhanced search filters, book categorization, or user role management with minimal code restructuring.
- **Scalability**: Capable of handling an increasing number of users and books as the library grows.

### Testing and Validation

- **Test Framework**: Includes custom test modules like `tester.c` to validate core functionalities, ensuring code reliability.
- **Error Logging**: Provides detailed error outputs for debugging and system monitoring.

---

## Compilation

To compile the server and client, follow these steps:

1. **Compile the Server:**

   Open a terminal and run the following command to run the server:

   ```bash
   gcc Database/User.c Database/pds.c Database/bst.c server.c -lpthread -o server
   ./server
   
 2. **Compile the Client:**

    Open another terminal  and run the following command to run the client:
       ```bash
     gcc -o logic client.c logic.c
     ./logic

If you would like to contribute, feel free to fork the repository and submit a pull request!

## Repository Link

Check out the project repository on GitHub:  
[Online Library Management System](https://github.com/vasuganesha2/Online-Library-Management-System-)


