/*
 * main.c — Entry point for the TP ADDS project
 *
 * This file handles all user interaction through a terminal menu.
 * It lets the user:
 *   1. Choose whether the two paragraphs come from the same file or different files
 *   2. Enter file name(s) and paragraph numbers
 *   3. Select a set operation: Union, Intersection, or Difference
 *   4. Display the result as a sorted set of words
 *   5. Repeat or exit
 *
 * All set operations are delegated to functions defined in bib.c / bib.h.
 * The paragraphs are loaded, converted to lowercase, then processed as word sets.
 *
 * Authors: Chibout Wahb & Chaib Mohamed — ESI Algiers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "main.h"

/* ─── ANSI Color Codes ────────────────────────────────────────────────────────
 * These macros define terminal escape sequences for colored output.
 * They are used to make the menu visually clear and readable.
 */
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"
#define RED     "\033[91m"
#define GREEN   "\033[92m"
#define YELLOW  "\033[93m"
#define CYAN    "\033[96m"
#define MAGENTA "\033[95m"
#define WHITE   "\033[97m"

/* ─── print_sep ───────────────────────────────────────────────────────────────
 * Prints a horizontal separator line in cyan.
 * Used to visually divide sections of the menu.
 */
static void print_sep() {
    printf(CYAN "  ------------------------------------------\n" RESET);
}

/* ─── print_header ────────────────────────────────────────────────────────────
 * Clears the terminal screen and prints the program's title banner.
 * Sets the console to UTF-8 on Windows (chcp 65001) to handle special chars.
 * Called once at startup and again after the user chooses to repeat.
 */
static void print_header() {
    system("chcp 65001 > nul");
    system("cls || clear");
    printf("\n");
    printf(MAGENTA BOLD "  ==========================================\n" RESET);
    printf(MAGENTA BOLD "        TP ADDS -- ESI ALGIERS\n" RESET);
    printf(YELLOW        "    Edited by: Chibout Wahb & Chaib Mohamed\n" RESET);
    printf(DIM           "    Algorithmics & Dynamic Data Structures\n" RESET);
    printf(MAGENTA BOLD "  ==========================================\n\n" RESET);
}

/* YOUR ORIGINAL CODE - UNTOUCHED BELOW */

/* ─── read_int ────────────────────────────────────────────────────────────────
 * Reads a single integer from stdin.
 * Flushes the rest of the input line afterwards to avoid leftover characters
 * causing problems on the next scanf/fgets call.
 * Returns -1 if reading fails.
 */
static int read_int() {
    int v;
    int result = scanf("%d", &v);
    while (getchar() != '\n');   // ← always flush, even on failure
    if (result != 1) return -1;
    return v;
}

/* ─── main ────────────────────────────────────────────────────────────────────
 * Program entry point.
 * Runs a loop that:
 *   1. Asks if both paragraphs are in the same file or different files
 *   2. Reads the file name(s) and paragraph numbers
 *   3. Asks which set operation to perform (Union / Intersection / Difference)
 *   4. Loads paragraphs using get_para(), converts them to lowercase
 *   5. Handles the special case where both paragraphs are identical
 *   6. Calls the appropriate set function and prints the result via inorder_traversal()
 *   7. Frees all allocated memory and asks the user to repeat or exit
 */
int main() {
    print_header();
    int running = 1;

    while (running) {
        char name1[200], name2[200];
        int para1, para2;
        int same_file = 0;
        int op = 0;

        /* ── Step 1: Same file or different files? ── */
        while (1) {
            print_sep();
            printf(WHITE "  Are the paragraphs in the same file?\n" RESET);
            printf(GREEN  "  [ 1 ]" WHITE " Yes   " RESET);
            printf(CYAN   "[ 2 ]" WHITE " No   " RESET);
            printf(RED    "[ 0 ]" WHITE " Exit\n" RESET);
            print_sep();
            printf(WHITE BOLD "  Your choice: " RESET);
            same_file = read_int();
            if (same_file == 0) return 0;
            if (same_file == 1 || same_file == 2) break;
            printf(RED "  Invalid choice. Try again.\n" RESET);
        }
int i;
        /* ── Step 2: Read file name(s) ── */
        if (same_file == 1) {
            /* Single file: copy the name into both name1 and name2 */
            printf(CYAN "  Enter file name (or full path): " RESET);
            fgets(name1, sizeof(name1), stdin);
            for (i = 0; name1[i]; i++) {
                if (name1[i] == '\n') { name1[i] = '\0'; break; }
            }
            strcpy(name2, name1);
        } else {
            /* Two different files */
            printf(CYAN "  Enter first file name (or full path): " RESET);
            fgets(name1, sizeof(name1), stdin);
            for (i = 0; name1[i]; i++) {
                if (name1[i] == '\n') { name1[i] = '\0'; break; }
            }
            printf(CYAN "  Enter second file name (or full path): " RESET);
            fgets(name2, sizeof(name2), stdin);
            for (i = 0; name2[i]; i++) {
                if (name2[i] == '\n') { name2[i] = '\0'; break; }
            }
        }

        /* ── Step 3: Read paragraph numbers ── */
        while (1) {
            printf(YELLOW "  Enter first paragraph number " RESET "(0 = exit): ");
            para1 = read_int();
            if (para1 == 0) return 0;
            if (para1 > 0) break;
            printf(RED "  Invalid number. Try again.\n" RESET);
        }

        while (1) {
            printf(YELLOW "  Enter second paragraph number " RESET "(0 = exit): ");
            para2 = read_int();
            if (para2 == 0) return 0;
            if (para2 > 0) break;
            printf(RED "  Invalid number. Try again.\n" RESET);
        }

        /* ── Step 4: Choose the set operation ── */
        while (1) {
            printf("\n");
            print_sep();
            printf(CYAN BOLD "         Choose Operation\n" RESET);
            print_sep();
            printf(GREEN  "  [ 1 ]" WHITE " Union          (A u B)\n" RESET);
            printf(YELLOW "  [ 2 ]" WHITE " Intersection   (A n B)\n" RESET);
            printf(CYAN   "  [ 3 ]" WHITE " Difference     (A \\ B)\n" RESET);
            printf(RED    "  [ 0 ]" WHITE " Exit\n" RESET);
            print_sep();
            printf(WHITE BOLD "  Your choice: " RESET);
            op = read_int();
            if (op == 0) return 0;
            if (op >= 1 && op <= 3) break;
            printf(RED "  Invalid choice. Try again.\n" RESET);
        }

        /* ── Step 5: Load paragraphs from file(s) ── */
        char *text1 = get_para(name1, para1);
        char *text2 = get_para(name2, para2);

        /* Normalize to lowercase for case-insensitive comparison */
        if (text1) to_lowercase(text1);
        if (text2) to_lowercase(text2);

        /* ── Step 6: Compare paragraphs and run the operation ── */
        int cmp = 1;
        if (text1 && text2) cmp = strcmp(text1, text2);

        if (text1 && text2 && cmp != 0) {
            /* The two paragraphs are different: run the chosen operation normally */

            /* Work on copies since strtok modifies strings in place */
            char *copy1 = malloc(strlen(text1) + 1);
            char *copy2 = malloc(strlen(text2) + 1);
            strcpy(copy1, text1);
            strcpy(copy2, text2);

            node_tree *R = NULL; 

            if (op == 1) {
                union_para(copy1, copy2, &R);
                printf(GREEN BOLD "\n  Union of paragraphs %d and %d:\n" RESET, para1, para2);
            } else if (op == 2) {
                intersection(copy1, copy2, &R);
                if(R == NULL) {
                    /* Intersection returned no common words */
                    printf(YELLOW BOLD "\n  No common words between paragraphs %d and %d.\n" RESET, para1, para2);
                    free(copy1);
                    free(copy2);
                    continue;
                }
                else {
                printf(YELLOW BOLD "\n  Intersection of paragraphs %d and %d:\n" RESET, para1, para2);
                }
            } else {
                difference(copy1, copy2, &R);
                printf(CYAN BOLD "\n  Difference of paragraphs %d and %d (A\\B):\n" RESET, para1, para2);
            }

            /* Print the result BST in alphabetical order as a set */
            print_sep();
            inorder_traversal(R);
            print_sep();
            free_tree(&R);

            free(copy1);
            free(copy2);

        } else {
            /* ── Special case: paragraphs are identical or one failed to load ── */
            node_tree *R = NULL;
            if(cmp == 0) {
                /* Both paragraphs have the same content */
                printf(YELLOW BOLD "\n  Both paragraphs are identical:\n" RESET);
                print_sep();
                if (op == 1) {
                    /* Union of identical sets = the set itself */
                printf(GREEN BOLD "\n  Union of paragraphs %d and %d:\n" RESET, para1, para2);
                separate_tree(text1, &R);
                inorder_traversal(R);
            } else if (op == 2) {
                    /* Intersection of identical sets = the set itself */
                separate_tree(text1, &R);
                printf(YELLOW BOLD "\n  Intersection of paragraphs %d and %d:\n" RESET, para1, para2);
                inorder_traversal(R);
            } else {
                /* Difference A \ A = empty set */
                printf(CYAN BOLD "\n  Difference of paragraphs  (A\\A) is empty \n" RESET);
            }
                print_sep();
                
                free_tree(&R);
            } else {
                /* One or both paragraphs could not be loaded */
                printf(RED "  Could not load paragraph(s). Check file path and number.\n" RESET);
            }
            
        }

        free(text1);
        free(text2);

        /* ── Step 7: Ask the user to repeat or exit ── */
        while (1) {
            int again;
            printf("\n");
            print_sep();
            printf(WHITE "  Run again? " GREEN "[ 1 ] Yes   " RED "[ 0 ] Exit\n" RESET);
            print_sep();
            printf(WHITE BOLD "  Your choice: " RESET);
            again = read_int();
            if (again == 0) { running = 0; break; }
            if (again == 1) { print_header(); break; }
            printf(RED "  Invalid choice. Try again.\n" RESET);
        }
    }

    printf(MAGENTA BOLD "\n  Goodbye! -- Chibout Wahb & Chaib Mohamed\n\n" RESET);
    return 0;
}