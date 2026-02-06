# Library Management System

A complete library management system written in C for recording and managing authors, books, and their relationships.  
This system allows users to maintain a database of authors and books, link them together, and perform various operations like searching and deleting records.

---

## Features

- **Add New Author**: Register new authors with unique IDs  
- **Add New Book**: Register new books with release dates and prices, and link them to existing or new authors  
- **Search Author**: Find authors by surname and view their books  
- **Search Book**: Find books by title and view their authors  
- **Delete Author**: Remove authors and automatically delete their books from the system  
- **Delete Book**: Remove books and update author records  
- **Persistent Storage**: All data is saved to and loaded from text files  
- **Dynamic Memory Management**: Efficient memory allocation and resizing  

---

## Data Structures

The system uses three main structures:

```c
typedef struct author {
    int writer_id;
    char *surname;
    char *name;
    int num_of_books;
} author;

typedef struct book {
    char *title;
    int release_date; // Format: DDMMYYYY as integer
    float price;
} book;

typedef struct writes {
    int writer_id;
    char *title;
} writes;
```

## Project Structure
project/
├── library.c        # Main source code
├── authors.txt      # Authors database
├── books.txt        # Books database
├── writes.txt       # Author-book relationships
└── README.md        # This file

## File Formats
authors.txt
=== Authors Database ===
Total authors: 3
Author ID: 1
Author's Surname: Smith
Author's Name: John
Number of books: 2

books.txt
=== Books Database ===
Total books: 2
Book Title: The Great Novel
Book Release Date: 15/05/2020
Book Price: 19.99

writes.txt
=== Author-Book Relations ===
Author's ID: 1
Title of book: The Great Novel


--- Compilation and Execution
gcc library.c -o library

--- Run the program
Linux / macOS
./library

--- Windows
library.exe

--- Compile with all warnings
gcc -Wall -Wextra library.c -o library



Menu Options
===== LIBRARY MANAGEMENT SYSTEM =====
1. Insert new author
2. Insert new book
3. Search a writer record
4. Search a book record
5. Delete a writer record
6. Delete a book record
7. Quit
=====================================


Usage Instructions

If authors.txt, books.txt, or writes.txt do not exist, the program will create them automatically.

Adding Data  
Add authors first (Option 1)
Add books and link them to existing authors (Option 2)

Searching
Search authors by surname (Option 3)
Search books by title (Option 4)

Deleting
Delete authors by ID (Option 5)
Delete books by title (Option 6)

Data Persistence
All data is automatically saved when you quit (Option 7)

Memory Management
Dynamic array resizing when capacity is reached
Proper memory allocation and deallocation
String duplication using strdup() for safe string handling
Automatic cleanup on program exit

Error Handling
File opening errors are caught and reported
Invalid user input is validated (dates, prices)
Memory allocation failures are handled gracefully
Nonexistent records are reported clearly

Requirements
C Compiler (GCC recommended)
Standard C Libraries (stdio.h, stdlib.h, string.h)
Any operating system (Linux, macOS, Windows)

Notes for Developers
Uses static global variables for the main arrays
File I/O is done in plain text for easy inspection
release_date is stored as an integer in DDMMYYYY format
Author IDs are auto-generated based on existing records
num_of_books is automatically updated when books are added/deleted

License
This project is for educational purposes

Author
Ioannis Charatsiaris
University Of Thesaaly, Department Computer Science with Application in Biomedicine
