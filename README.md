# BST Text Set Operations

A university project developed in C at ESI Algiers, implementing Binary Search Trees (BSTs) to perform set operations on words extracted from text paragraphs.

## Features

* **Binary Search Tree (BST):** Store and organize unique words.
* **Union:** Find all unique words appearing in either of two paragraphs.
* **Intersection:** Find words common to both paragraphs.
* **Difference:** Find words present in the first paragraph but absent from the second.
* **Text Processing:** Extract words from paragraphs and read paragraphs from text files.
* **Memory Management:** Allocate and free memory dynamically.

## Technologies

* C
* Data Structures and Algorithms
* Binary Search Trees
* Dynamic Memory Allocation
* String Processing

## Project Structure

* `main.c` — Main program.
* `main.h` — Header file, if used by the project.
* `bib.c` — Core library implementing the BST and set operations.
* `A.text` — Input text file.
* `B.text` — Input text file.

## How to Compile and Run

Compile the source files using GCC. For example:

```bash
gcc main.c bib.c -o bst_project
```

Run the program:

```bash
./bst_project
```

On Windows, run the generated executable:

```bash
bst_project.exe
```

**Note:** The compilation command may need to be adjusted depending on the project's actual file structure and function declarations.

## Authors

* Wahb Chibout
* Mohamed Chaib

Developed as a university practical assignment at ESI Algiers.

