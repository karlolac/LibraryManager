#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

#define DATA_FILE "library.dat"

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_menu(void) {
    printf("\n--- LIBRARY MANAGER ---\n");
    printf("1. Prikazi sve knjige\n");
    printf("2. Dodaj novu knjigu\n");
    printf("3. Obrisi knjigu po ID-u\n");
    printf("4. Promijeni status knjige\n");
    printf("5. Izlaz\n");
    printf("Odabir: ");
}

int main(void) {
    Library lib;
    library_init(&lib);

    if (library_load_from_file(&lib, DATA_FILE)) {
        printf("Podaci ucitani iz '%s'. Ukupno knjiga: %zu\n", DATA_FILE, lib.count);
    }
    else {
        printf("Zapoceta je nova knjiznica.\n");
    }

    int choice = 0;
    int next_id = 1;

    for (size_t i = 0; i < lib.count; i++) {
        if (lib.books[i].id >= next_id) {
            next_id = lib.books[i].id + 1;
        }
    }

    while (choice != 5) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
        case 1:
            library_print_all(&lib);
            break;

        case 2: {
            Book new_book;
            new_book.id = next_id++;

            printf("Unesite naslov: ");
            fgets(new_book.title, sizeof(new_book.title), stdin);
            new_book.title[strcspn(new_book.title, "\n")] = 0;

            printf("Unesite autora: ");
            fgets(new_book.author, sizeof(new_book.author), stdin);
            new_book.author[strcspn(new_book.author, "\n")] = 0;

            printf("Unesite godinu izdanja: ");
            scanf("%d", &new_book.year);

            printf("Unesite ocjenu (1-5): ");
            scanf("%d", &new_book.rating);

            printf("Odaberite status (0: Zelja, 1: U citanju, 2: Procitano): ");
            int status_val;
            scanf("%d", &status_val);
            new_book.status = (ReadingStatus)(status_val >= 0 && status_val <= 2 ? status_val : 0);

            if (library_add(&lib, new_book)) {
                printf("Knjiga uspjesno dodana s ID-em: %d\n", new_book.id);
            }
            break;
        }

        case 3: {
            int id;
            printf("Unesite ID knjige za brisanje: ");
            scanf("%d", &id);
            if (library_remove(&lib, id)) {
                printf("Knjiga s ID-em %d je obrisana.\n", id);
            }
            else {
                printf("Knjiga s navedenim ID-em nije pronadjena.\n");
            }
            break;
        }

        case 4: {
            int id;
            printf("Unesite ID knjige: ");
            scanf("%d", &id);
            Book* b = library_find_by_id(&lib, id);
            if (b) {
                printf("Novi status (0: Zelja, 1: U citanju, 2: Procitano): ");
                int status_val;
                scanf("%d", &status_val);
                b->status = (ReadingStatus)(status_val >= 0 && status_val <= 2 ? status_val : 0);
                printf("Status uspjesno azuriran.\n");
            }
            else {
                printf("Knjiga s navedenim ID-em nije pronadjena.\n");
            }
            break;
        }

        case 5:
            if (library_save_to_file(&lib, DATA_FILE)) {
                printf("Podaci uspjesno spremljeni u '%s'.\n", DATA_FILE);
            }
            printf("Dovidjenja!\n");
            break;

        default:
            printf("Neispravan odabir. Pokusajte ponovno.\n");
            break;
        }
    }

    library_free(&lib);
    return 0;
}