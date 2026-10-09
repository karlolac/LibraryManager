# Library Manager (C)

A modular CLI application in C for managing a personal book collection with dynamic memory scaling and binary persistence.

## Key Features

* **Dynamic Array Scaling:** Uses `realloc` to double memory capacity as needed ($\mathcal{O}(1)$ amortized insertion).
* **Binary Persistence:** Saves and loads library state directly via binary file operations (`fwrite`/`fread`).
* **Complete CRUD:** Add, list, search, update status, and remove entries by ID.
* **Memory Safe:** Ensures full memory deallocation (`free`) upon exit without leaks.

## Project Structure

```text
├── include/
│   └── library.h          # Header with structures and function declarations
├── src/
│   ├── library.c          # Core logic and file operations
│   └── Library Manager.c  # CLI menu loop and main entry point
├── Makefile               # Build script for GCC / Clang
└── README.md

How to Build and RunVisual Studio (Windows)Open Library Manager.sln.   Press Ctrl + F5 to run.

GCC / Make
Bash
make
./library_manager
