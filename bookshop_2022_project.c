#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct book
{ 
    char *title;
    int release_date;
    float price;
} book;

typedef struct author
{
    int writer_id;
    char *surname;
    char *name;
    int num_of_books;
} author;

typedef struct writes
{
    int writer_id;
    char *title;
} writes;


void printAuthors(struct author *author_arr, int size);
int counter_author();
int counter_books();


static author *author_arr = NULL;
static book *book_arr = NULL;
static writes *writes_arr = NULL;
static int author_count = 0;
static int book_count = 0;
static int writes_count = 0;
static int author_capacity = 10;
static int book_capacity = 10;
static int writes_capacity = 10;

// Πρόσθετες συναρτήσεις
void initialize_arrays();
void free_all_memory();
void load_authors_from_file();
void load_books_from_file();
void load_writes_from_file();
void save_authors_to_file();
void save_books_to_file();
void save_writes_to_file();
void insert_new_author();
void insert_new_book();
void search_author();
void search_book();
void delete_author();
void delete_book();
void display_main_menu();
void resize_authors_array();
void resize_books_array();
void resize_writes_array();
void update_author_book_count();

// Βελτιωμένες εκδόσεις των counter συναρτήσεων (χωρίς να γράφουν στο αρχείο)
int count_authors_in_file() {
    FILE *file = fopen("authors.txt", "r");
    if (file == NULL) {
        return 0;
    }
    
    int count = 0;
    char buffer[256];
    
    // Skip first line if it contains header
    fgets(buffer, sizeof(buffer), file);
    
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strstr(buffer, "Author ID:") != NULL) {
            count++;
        }
    }
    
    fclose(file);
    return count;
}

int count_books_in_file() {
    FILE *file = fopen("books.txt", "r");
    if (file == NULL) {
        return 0;
    }
    
    int count = 0;
    char buffer[256];
    
    // Skip first line if it contains header
    fgets(buffer, sizeof(buffer), file);
    
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strstr(buffer, "Book Title:") != NULL) {
            count++;
        }
    }
    
    fclose(file);
    return count;
}

int main() {
    initialize_arrays();
    load_authors_from_file();
    load_books_from_file();
    load_writes_from_file();
    update_author_book_count();
    
    int choice;
    do {
        display_main_menu();
        printf("Please choose an option: ");
        scanf("%d", &choice);
        getchar(); // Καθαρισμός buffer
        
        switch(choice) {
            case 1:
                insert_new_author();
                break;
            case 2:
                insert_new_book();
                break;
            case 3:
                search_author();
                break;
            case 4:
                search_book();
                break;
            case 5:
                delete_author();
                break;
            case 6:
                delete_book();
                break;
            case 7:
                printf("Saving data and exiting...\n");
                save_authors_to_file();
                save_books_to_file();
                save_writes_to_file();
                break;
            default:
                printf("Invalid choice! Please enter 1-7.\n");
        }
    } while(choice != 7);
    
    free_all_memory();
    return 0;
}

void initialize_arrays() {
    author_arr = (author*)malloc(author_capacity * sizeof(author));
    for(int i = 0; i < author_capacity; i++) {
        author_arr[i].surname = NULL;
        author_arr[i].name = NULL;
        author_arr[i].num_of_books = 0;
    }
    
    book_arr = (book*)malloc(book_capacity * sizeof(book));
    for(int i = 0; i < book_capacity; i++) {
        book_arr[i].title = NULL;
    }
    
    writes_arr = (writes*)malloc(writes_capacity * sizeof(writes));
    for(int i = 0; i < writes_capacity; i++) {
        writes_arr[i].title = NULL;
    }
}

void free_all_memory() {
    for(int i = 0; i < author_count; i++) {
        free(author_arr[i].surname);
        free(author_arr[i].name);
    }
    free(author_arr);
    
    for(int i = 0; i < book_count; i++) {
        free(book_arr[i].title);
    }
    free(book_arr);
    
    for(int i = 0; i < writes_count; i++) {
        free(writes_arr[i].title);
    }
    free(writes_arr);
}

void resize_authors_array() {
    author_capacity *= 2;
    author_arr = (author*)realloc(author_arr, author_capacity * sizeof(author));
    for(int i = author_count; i < author_capacity; i++) {
        author_arr[i].surname = NULL;
        author_arr[i].name = NULL;
        author_arr[i].num_of_books = 0;
    }
}

void resize_books_array() {
    book_capacity *= 2;
    book_arr = (book*)realloc(book_arr, book_capacity * sizeof(book));
    for(int i = book_count; i < book_capacity; i++) {
        book_arr[i].title = NULL;
    }
}

void resize_writes_array() {
    writes_capacity *= 2;
    writes_arr = (writes*)realloc(writes_arr, writes_capacity * sizeof(writes));
    for(int i = writes_count; i < writes_capacity; i++) {
        writes_arr[i].title = NULL;
    }
}

void load_authors_from_file() {
    FILE *file = fopen("authors.txt", "r");
    if(!file) {
        printf("Warning: authors.txt not found. Starting with empty authors list.\n");
        return;
    }
    
    char buffer[256];
    // Skip first line if it's a header/counter
    fgets(buffer, sizeof(buffer), file);
    
    while(fgets(buffer, sizeof(buffer), file)) {
        if(strstr(buffer, "Author ID:")) {
            if(author_count >= author_capacity) {
                resize_authors_array();
            }
            
            int id;
            sscanf(buffer, "Author ID: %d", &id);
            
            // Read surname
            fgets(buffer, sizeof(buffer), file);
            if(strstr(buffer, "Author's Surname:")) {
                author_arr[author_count].writer_id = id;
                char temp[100];
                sscanf(buffer, "Author's Surname: %[^\n]", temp);
                author_arr[author_count].surname = strdup(temp);
            }
            
            // Read name
            fgets(buffer, sizeof(buffer), file);
            if(strstr(buffer, "Author's Name:")) {
                char temp[100];
                sscanf(buffer, "Author's Name: %[^\n]", temp);
                author_arr[author_count].name = strdup(temp);
                author_arr[author_count].num_of_books = 0;
                author_count++;
            }
        }
    }
    
    fclose(file);
}

void load_books_from_file() {
    FILE *file = fopen("books.txt", "r");
    if(!file) {
        printf("Warning: books.txt not found. Starting with empty books list.\n");
        return;
    }
    
    char buffer[256];
    // Skip first line if it's a header/counter
    fgets(buffer, sizeof(buffer), file);
    
    while(fgets(buffer, sizeof(buffer), file)) {
        if(strstr(buffer, "Book Title:")) {
            if(book_count >= book_capacity) {
                resize_books_array();
            }
            
            char temp[100];
            sscanf(buffer, "Book Title: %[^\n]", temp);
            book_arr[book_count].title = strdup(temp);
            
            // Read release date
            fgets(buffer, sizeof(buffer), file);
            if(strstr(buffer, "Book Release Date:")) {
                int day, month, year;
                sscanf(buffer, "Book Release Date: %d/%d/%d", &day, &month, &year);
                book_arr[book_count].release_date = day * 10000 + month * 100 + year;
            }
            
            // Read price
            fgets(buffer, sizeof(buffer), file);
            if(strstr(buffer, "Book Price:")) {
                sscanf(buffer, "Book Price: %f", &book_arr[book_count].price);
                book_count++;
            }
        }
    }
    
    fclose(file);
}

void load_writes_from_file() {
    FILE *file = fopen("writes.txt", "r");
    if(!file) {
        printf("Warning: writes.txt not found.\n");
        return;
    }
    
    char buffer[256];
    while(fgets(buffer, sizeof(buffer), file)) {
        if(strstr(buffer, "Author's ID:")) {
            if(writes_count >= writes_capacity) {
                resize_writes_array();
            }
            
            int id;
            sscanf(buffer, "Author's ID: %d", &id);
            writes_arr[writes_count].writer_id = id;
            
            // Read title
            fgets(buffer, sizeof(buffer), file);
            if(strstr(buffer, "Title of book:")) {
                char temp[100];
                sscanf(buffer, "Title of book: %[^\n]", temp);
                writes_arr[writes_count].title = strdup(temp);
                writes_count++;
            }
        }
    }
    
    fclose(file);
}

void update_author_book_count() {
    // Αρχικοποίηση
    for(int i = 0; i < author_count; i++) {
        author_arr[i].num_of_books = 0;
    }
    
    // Μετρήστε βιβλία για κάθε συγγραφέα
    for(int i = 0; i < writes_count; i++) {
        for(int j = 0; j < author_count; j++) {
            if(author_arr[j].writer_id == writes_arr[i].writer_id) {
                author_arr[j].num_of_books++;
                break;
            }
        }
    }
}

void save_authors_to_file() {
    FILE *file = fopen("authors.txt", "w");
    if(!file) {
        printf("Error saving authors to file!\n");
        return;
    }
    
    fprintf(file, "=== Authors Database ===\n");
    fprintf(file, "Total authors: %d\n", author_count);
    
    for(int i = 0; i < author_count; i++) {
        fprintf(file, "Author ID: %d\n", author_arr[i].writer_id);
        fprintf(file, "Author's Surname: %s\n", author_arr[i].surname);
        fprintf(file, "Author's Name: %s\n", author_arr[i].name);
        fprintf(file, "Number of books: %d\n", author_arr[i].num_of_books);
        fprintf(file, "---------------------\n");
    }
    
    fclose(file);
}

void save_books_to_file() {
    FILE *file = fopen("books.txt", "w");
    if(!file) {
        printf("Error saving books to file!\n");
        return;
    }
    
    fprintf(file, "=== Books Database ===\n");
    fprintf(file, "Total books: %d\n", book_count);
    
    for(int i = 0; i < book_count; i++) {
        int year = book_arr[i].release_date % 10000;
        int month = (book_arr[i].release_date / 10000) % 100;
        int day = book_arr[i].release_date / 1000000;
        
        fprintf(file, "Book Title: %s\n", book_arr[i].title);
        fprintf(file, "Book Release Date: %02d/%02d/%04d\n", day, month, year);
        fprintf(file, "Book Price: %.2f\n", book_arr[i].price);
        fprintf(file, "---------------------\n");
    }
    
    fclose(file);
}

void save_writes_to_file() {
    FILE *file = fopen("writes.txt", "w");
    if(!file) {
        printf("Error saving writes to file!\n");
        return;
    }
    
    fprintf(file, "=== Author-Book Relations ===\n");
    
    for(int i = 0; i < writes_count; i++) {
        fprintf(file, "Author's ID: %d\n", writes_arr[i].writer_id);
        fprintf(file, "Title of book: %s\n", writes_arr[i].title);
        fprintf(file, "---------------------\n");
    }
    
    fclose(file);
}

void display_main_menu() {
    printf("\n===== LIBRARY MANAGEMENT SYSTEM =====\n");
    printf("1. Insert new author\n");
    printf("2. Insert new book\n");
    printf("3. Search a writer record\n");
    printf("4. Search a book record\n");
    printf("5. Delete a writer record\n");
    printf("6. Delete a book record\n");
    printf("7. Quit\n");
    printf("=====================================\n");
}

void insert_new_author() {
    if(author_count >= author_capacity) {
        resize_authors_array();
    }
    
    printf("\n--- Insert New Author ---\n");
    
    // Βρείτε το επόμενο διαθέσιμο ID
    int new_id = 1;
    for(int i = 0; i < author_count; i++) {
        if(author_arr[i].writer_id >= new_id) {
            new_id = author_arr[i].writer_id + 1;
        }
    }
    
    author_arr[author_count].writer_id = new_id;
    
    // Πάρτε επώνυμο
    printf("Enter surname: ");
    char surname[100];
    fgets(surname, sizeof(surname), stdin);
    surname[strcspn(surname, "\n")] = '\0';
    author_arr[author_count].surname = strdup(surname);
    
    // Πάρτε όνομα
    printf("Enter name: ");
    char name[100];
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    author_arr[author_count].name = strdup(name);
    
    author_arr[author_count].num_of_books = 0;
    author_count++;
    
    printf("Author added successfully with ID: %d\n", new_id);
    save_authors_to_file();
}

void insert_new_book() {
    printf("\n--- Insert New Book ---\n");
    
    if(book_count >= book_capacity) {
        resize_books_array();
    }
    
    // Πάρτε τίτλο βιβλίου
    printf("Enter book title: ");
    char title[100];
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0';
    book_arr[book_count].title = strdup(title);
    
    // Πάρτε ημερομηνία κυκλοφορίας
    int day, month, year;
    do {
        printf("Enter release date (dd/mm/yyyy): ");
        scanf("%d/%d/%d", &day, &month, &year);
        getchar();
        
        if(day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100) {
            printf("Invalid date! Please try again.\n");
        }
    } while(day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100);
    
    book_arr[book_count].release_date = day * 10000 + month * 100 + year;
    
    // Πάρτε τιμή
    printf("Enter price: ");
    scanf("%f", &book_arr[book_count].price);
    getchar();
    
    // Σύνδεση βιβλίου με συγγραφείς
    printf("\nLink book to author(s). Enter author surname (empty to finish):\n");
    
    char surname[100];
    while(1) {
        printf("Author surname: ");
        fgets(surname, sizeof(surname), stdin);
        surname[strcspn(surname, "\n")] = '\0';
        
        if(strlen(surname) == 0) {
            break;
        }
        
        // Αναζήτηση συγγραφέα
        int found = 0;
        for(int i = 0; i < author_count; i++) {
            if(strcmp(author_arr[i].surname, surname) == 0) {
                found = 1;
                
                // Προσθήκη στον πίνακα writes
                if(writes_count >= writes_capacity) {
                    resize_writes_array();
                }
                
                writes_arr[writes_count].writer_id = author_arr[i].writer_id;
                writes_arr[writes_count].title = strdup(title);
                writes_count++;
                
                author_arr[i].num_of_books++;
                
                printf("Linked to author: %s %s (ID: %d)\n", 
                       author_arr[i].surname, author_arr[i].name, author_arr[i].writer_id);
                break;
            }
        }
        
        if(!found) {
            printf("Author not found. Would you like to add this author? (y/n): ");
            char choice;
            scanf("%c", &choice);
            getchar();
            
            if(choice == 'y' || choice == 'Y') {
                insert_new_author(); // Προσθήκη του νέου συγγραφέα
                
                // Σύνδεση με τον τελευταίο συγγραφέα που προστέθηκε
                writes_arr[writes_count].writer_id = author_arr[author_count-1].writer_id;
                writes_arr[writes_count].title = strdup(title);
                writes_count++;
                
                author_arr[author_count-1].num_of_books++;
            }
        }
    }
    
    book_count++;
    printf("Book added successfully!\n");
    
    save_books_to_file();
    save_writes_to_file();
    save_authors_to_file(); // Για να ενημερωθεί το num_of_books
}

void search_author() {
    printf("\n--- Search Author ---\n");
    printf("Enter surname to search: ");
    char surname[100];
    fgets(surname, sizeof(surname), stdin);
    surname[strcspn(surname, "\n")] = '\0';
    
    int found = 0;
    for(int i = 0; i < author_count; i++) {
        if(strcmp(author_arr[i].surname, surname) == 0) {
            printf("\nFound author:\n");
            printf("ID: %d\n", author_arr[i].writer_id);
            printf("Surname: %s\n", author_arr[i].surname);
            printf("Name: %s\n", author_arr[i].name);
            printf("Number of books: %d\n", author_arr[i].num_of_books);
            
            // Βρείτε βιβλία αυτού του συγγραφέα
            printf("Books by this author:\n");
            int book_found = 0;
            for(int j = 0; j < writes_count; j++) {
                if(writes_arr[j].writer_id == author_arr[i].writer_id) {
                    printf("  - %s\n", writes_arr[j].title);
                    book_found = 1;
                }
            }
            if(!book_found) {
                printf("  No books found\n");
            }
            
            found = 1;
        }
    }
    
    if(!found) {
        printf("Author not found.\n");
    }
}

void search_book() {
    printf("\n--- Search Book ---\n");
    printf("Enter book title to search: ");
    char title[100];
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0';
    
    int found = 0;
    
    // Αναζήτηση στον πίνακα βιβλίων
    for(int i = 0; i < book_count; i++) {
        if(strstr(book_arr[i].title, title) != NULL) {
            printf("\nFound book:\n");
            printf("Title: %s\n", book_arr[i].title);
            
            int year = book_arr[i].release_date % 10000;
            int month = (book_arr[i].release_date / 10000) % 100;
            int day = book_arr[i].release_date / 1000000;
            printf("Release Date: %02d/%02d/%04d\n", day, month, year);
            printf("Price: %.2f\n", book_arr[i].price);
            
            // Βρείτε συγγραφείς αυτού του βιβλίου
            printf("Authors:\n");
            int author_found = 0;
            for(int j = 0; j < writes_count; j++) {
                if(strcmp(writes_arr[j].title, book_arr[i].title) == 0) {
                    // Βρείτε τα στοιχεία του συγγραφέα
                    for(int k = 0; k < author_count; k++) {
                        if(author_arr[k].writer_id == writes_arr[j].writer_id) {
                            printf("  - %s %s (ID: %d)\n", 
                                   author_arr[k].surname, author_arr[k].name, 
                                   author_arr[k].writer_id);
                            author_found = 1;
                            break;
                        }
                    }
                }
            }
            if(!author_found) {
                printf("  No authors found\n");
            }
            
            found = 1;
        }
    }
    
    if(!found) {
        printf("Book not found.\n");
    }
}

void delete_author() {
    printf("\n--- Delete Author ---\n");
    printf("Enter author ID to delete: ");
    int id;
    scanf("%d", &id);
    getchar();
    
    int found_index = -1;
    for(int i = 0; i < author_count; i++) {
        if(author_arr[i].writer_id == id) {
            found_index = i;
            break;
        }
    }
    
    if(found_index == -1) {
        printf("Author with ID %d not found.\n", id);
        return;
    }
    
    printf("Are you sure you want to delete author: %s %s (ID: %d)? (y/n): ",
           author_arr[found_index].surname, author_arr[found_index].name, id);
    char choice;
    scanf("%c", &choice);
    getchar();
    
    if(choice == 'y' || choice == 'Y') {
        // Ελευθέρωση μνήμης
        free(author_arr[found_index].surname);
        free(author_arr[found_index].name);
        
        // Μετατόπιση πίνακα
        for(int i = found_index; i < author_count - 1; i++) {
            author_arr[i] = author_arr[i + 1];
        }
        author_count--;
        
        // Διαγραφή σχετικών εγγραφών writes
        for(int i = 0; i < writes_count; i++) {
            if(writes_arr[i].writer_id == id) {
                free(writes_arr[i].title);
                // Μετατόπιση πίνακα writes
                for(int j = i; j < writes_count - 1; j++) {
                    writes_arr[j] = writes_arr[j + 1];
                }
                writes_count--;
                i--; // Ελέγξτε το ίδιο ευρετήριο ξανά μετά τη μετατόπιση
            }
        }
        
        printf("Author deleted successfully.\n");
        save_authors_to_file();
        save_writes_to_file();
    } else {
        printf("Deletion cancelled.\n");
    }
}

void delete_book() {
    printf("\n--- Delete Book ---\n");
    printf("Enter book title to delete: ");
    char title[100];
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0';
    
    int found_index = -1;
    for(int i = 0; i < book_count; i++) {
        if(strcmp(book_arr[i].title, title) == 0) {
            found_index = i;
            break;
        }
    }
    
    if(found_index == -1) {
        printf("Book '%s' not found.\n", title);
        return;
    }
    
    printf("Are you sure you want to delete book: %s? (y/n): ", title);
    char choice;
    scanf("%c", &choice);
    getchar();
    
    if(choice == 'y' || choice == 'Y') {
        // Ενημέρωση αριθμού βιβλίων συγγραφέων
        for(int i = 0; i < writes_count; i++) {
            if(strcmp(writes_arr[i].title, title) == 0) {
                for(int j = 0; j < author_count; j++) {
                    if(author_arr[j].writer_id == writes_arr[i].writer_id) {
                        author_arr[j].num_of_books--;
                        break;
                    }
                }
            }
        }
        
        // Ελευθέρωση μνήμης
        free(book_arr[found_index].title);
        
        // Μετατόπιση πίνακα
        for(int i = found_index; i < book_count - 1; i++) {
            book_arr[i] = book_arr[i + 1];
        }
        book_count--;
        
        // Διαγραφή σχετικών εγγραφών writes
        for(int i = 0; i < writes_count; i++) {
            if(strcmp(writes_arr[i].title, title) == 0) {
                free(writes_arr[i].title);
                // Μετατόπιση πίνακα writes
                for(int j = i; j < writes_count - 1; j++) {
                    writes_arr[j] = writes_arr[j + 1];
                }
                writes_count--;
                i--; // Ελέγξτε το ίδιο ευρετήριο ξανά μετά τη μετατόπιση
            }
        }
        
        printf("Book deleted successfully.\n");
        save_books_to_file();
        save_writes_to_file();
        save_authors_to_file(); // Για να ενημερωθεί το num_of_books
    } else {
        printf("Deletion cancelled.\n");
    }
}

void printAuthors(author *author_arr, int size) {
    for(int i = 0; i < size; i++) {
        printf("Author's ID: %d\n", author_arr[i].writer_id);
        printf("Author's Surname: %s\n", author_arr[i].surname);
        printf("Author's Name: %s\n", author_arr[i].name);
        printf("Number of books: %d\n", author_arr[i].num_of_books);
        printf("---------------------------\n");
    }
}
