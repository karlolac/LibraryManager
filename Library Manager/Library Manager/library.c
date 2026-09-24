#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

#define INITIAL_CAPACITY 4

void library_init(Library* lib) {
    lib->books = (Book*)malloc(INITIAL_CAPACITY * sizeof(Book));
    lib->count = 0;
    lib->capacity = lib->books ? INITIAL_CAPACITY : 0;
}

void library_free(Library* lib) {
    if (lib->books) {
        free(lib->books);
        lib->books = NULL;
    }
    lib->count = 0;
    lib->capacity = 0;
}

int library_add(Library* lib, Book book) {
    if (lib->count >= lib->capacity) {
        size_t new_capacity = (lib->capacity == 0) ? INITIAL_CAPACITY : lib->capacity * 2;
        Book* new_books = (Book*)realloc(lib->books, new_capacity * sizeof(Book));
        if (!new_books) {
            fprintf(stderr, "Pogreska: Nedovoljno memorije za prosirenje knjiznice.\n");
            return 0;
        }
        lib->books = new_books;
        lib->capacity = new_capacity;
    }

    lib->books[lib->count++] = book;
    return 1;
}

int library_remove(Library* lib, int id) {
    for (size_t i = 0; i < lib->count; i++) {
        if (lib->books[i].id == id) {
            for (size_t j = i; j < lib->count - 1; j++) {
                lib->books[j] = lib->books[j + 1];
            }
            lib->count--;
            return 1;
        }
    }
    return 0;
}

Book* library_find_by_id(Library* lib, int id) {
    for (size_t i = 0; i < lib->count; i++) {
        if (lib->books[i].id == id) {
            return &lib->books[i];
        }
    }
    return NULL;
}

const char* status_to_string(ReadingStatus status) {
    switch (status) {
    case WANT_TO_READ: return "Zelja";
    case READING:      return "U citanju";
    case COMPLETED:    return "Procitano";
    default:           return "Nepoznato";
    }
}

void library_print_all(const Library* lib) {
    if (lib->count == 0) {
        printf("\nKnjiznica je prazna.\n");
        return;
    }

    printf("\n========================================================================================\n");
    printf("%-5s | %-30s | %-20s | %-6s | %-6s | %-12s\n", "ID", "Naslov", "Autor", "Godina", "Ocjena", "Status");
    printf("----------------------------------------------------------------------------------------\n");
    for (size_t i = 0; i < lib->count; i++) {
        const Book* b = &lib->books[i];
        printf("%-5d | %-30.30s | %-20.20s | %-6d | %-6d | %-12s\n",
            b->id, b->title, b->author, b->year, b->rating, status_to_string(b->status));
    }
    printf("========================================================================================\n");
}

int library_save_to_file(const Library* lib, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) return 0;

    fwrite(&lib->count, sizeof(size_t), 1, file);
    if (lib->count > 0) {
        fwrite(lib->books, sizeof(Book), lib->count, file);
    }

    fclose(file);
    return 1;
}

int library_load_from_file(Library* lib, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return 0;

    size_t count = 0;
    if (fread(&count, sizeof(size_t), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    for (size_t i = 0; i < count; i++) {
        Book b;
        if (fread(&b, sizeof(Book), 1, file) == 1) {
            library_add(lib, b);
        }
    }

    fclose(file);
    return 1;
}