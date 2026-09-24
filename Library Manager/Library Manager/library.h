#ifndef LIBRARY_H
#define LIBRARY_H

#include <stddef.h>

typedef enum {
    WANT_TO_READ,
    READING,
    COMPLETED
} ReadingStatus;

typedef struct {
    int id;
    char title[100];
    char author[100];
    int year;
    int rating; 
    ReadingStatus status;
} Book;

typedef struct {
    Book* books;
    size_t count;
    size_t capacity;
} Library;

void library_init(Library* lib);
void library_free(Library* lib);
int library_add(Library* lib, Book book);
int library_remove(Library* lib, int id);
Book* library_find_by_id(Library* lib, int id);

void library_print_all(const Library* lib);
const char* status_to_string(ReadingStatus status);

int library_save_to_file(const Library* lib, const char* filename);
int library_load_from_file(Library* lib, const char* filename);

#endif 

