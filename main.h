/*
 * bib.c — Core library for the TP ADDS project
 *
 * This file implements a Binary Search Tree (BST) of words.
 * It provides all the building blocks needed to:
 *   - Build a BST from a paragraph of text
 *   - Perform set operations: Union, Intersection, Difference
 *   - Read a specific paragraph from a text file
 *   - Print and free the tree
 *
 * Authors: Chibout Wahb & Chaib Mohamed — ESI Algiers
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* ─── Node Structure ──────────────────────────────────────────────────────────
 * Each node in the BST holds:
 *   - word  : a dynamically allocated string
 *   - left  : pointer to the left child  (words that come before alphabetically)
 *   - right : pointer to the right child (words that come after alphabetically)
 */
typedef struct node_tree{
    char *word;
    struct node_tree *left;
    struct node_tree *right;
} node_tree;

/* ─── allocate_node ───────────────────────────────────────────────────────────
 * Allocates memory for a new tree node and initializes all its fields to NULL.
 * Returns 1 on success, 0 on failure.
 */
int allocate_node(node_tree **p){
    if (p==NULL) return 0;
    *p = malloc(sizeof(node_tree));
    if (*p==NULL) return 0;
    (*p)->word=NULL;
    (*p)->left=NULL;
    (*p)->right=NULL;
    return 1;
}

/* ─── allocate_word ───────────────────────────────────────────────────────────
 * Allocates memory for a string of `length` characters (+1 for the null terminator).
 * Returns 1 on success, 0 on failure.
 */
int allocate_word(char **p,size_t length){
    if (p==NULL) return 0;
    *p= malloc(length+1);
    if (*p==NULL) return 0;
    return 1;
}

/* ─── process ─────────────────────────────────────────────────────────────────
 * Prints the word stored in a node.
 * Used as a visit action during tree traversal.
 * Returns 1 on success, 0 if the node or word is NULL.
 */
int process(node_tree *R){
    if ((R==NULL) || R->word ==NULL) return 0;
    printf("%s\n", R->word);
    return 1;
}

/* ─── RC / LC ─────────────────────────────────────────────────────────────────
 * Accessor functions:
 *   RC(R) → returns the right child of R
 *   LC(R) → returns the left child of R
 * Both return NULL safely if R is NULL.
 */
node_tree *RC(node_tree *R){
    if (R==NULL) return NULL;
    return R->right;
}
node_tree *LC(node_tree *R){
    if (R==NULL) return NULL;
    return R->left;
}

/* ─── ass_rc / ass_lc / ass_val ──────────────────────────────────────────────
 * Setter functions (mutators):
 *   ass_rc(p, q) → sets q as the right child of p
 *   ass_lc(p, q) → sets q as the left child of p
 *   ass_val(p, w) → assigns word w to node p
 * All return 1 on success, 0 if p is NULL.
 */
int ass_rc(node_tree *p, node_tree *q){
    if (p==NULL) return 0;
    p->right = q;
    return 1;
}
int ass_lc(node_tree *p, node_tree *q){
    if (p==NULL) return 0;
    p->left = q;
    return 1;
}   
int ass_val(node_tree *p,char *w){
    if (p==NULL ) return 0;
    p->word = w;
    return 1;
}

/* ─── create_tree ─────────────────────────────────────────────────────────────
 * Initializes a new empty tree by allocating its root node.
 * The root's word/left/right are all NULL after this call.
 * Returns 1 on success, 0 on failure.
 */
int create_tree(node_tree **R){
    if (R==NULL) return 0;
    allocate_node(R); 
    return 1;  
}

/* ─── free_tree ───────────────────────────────────────────────────────────────
 * Recursively frees all nodes in the BST (post-order traversal):
 *   1. Free left subtree
 *   2. Free right subtree
 *   3. Free the word string of the current node
 *   4. Free the node itself and set pointer to NULL
 * Returns 1 on success, 0 if already NULL.
 */
int free_tree(node_tree **p){
    if ((p==NULL) || (*p==NULL)) return 0;
    free_tree(&((*p)->left));
    free_tree(&((*p)->right));
    free((*p)->word);
    (*p)->word=NULL;
    free(*p);
    *p=NULL;
    return 1;
}

/* ─── insert ──────────────────────────────────────────────────────────────────
 * Inserts node `p` into the BST rooted at `R` following BST ordering rules:
 *   - If p->word == R->word → duplicate, node p is freed and discarded
 *   - If p->word  > R->word → go right
 *   - If p->word  < R->word → go left
 * This ensures the tree never contains duplicate words (set property).
 * Returns 1 on insertion, 0 if duplicate or invalid input.
 */
int insert(node_tree *R,node_tree *p){
    if(p==NULL||R==NULL||R->word==NULL||p->word==NULL) return 0;
    int cmp=strcmp(p->word,R->word);
    if (cmp==0){
        free(p->word);
        free(p);
        return 0;
    } 
    if (cmp>0){
        if(RC(R)==NULL) ass_rc(R,p);
        else insert(RC(R),p);
    }
    else{
        if(LC(R)==NULL) ass_lc(R,p);
        else insert(LC(R),p);
    }
    return 1;
}

/* ─── first_inorder ───────────────────────────────────────────────────────────
 * Returns the leftmost (smallest) node in the subtree rooted at `node`.
 * Used by delete_node to find the in-order successor.
 */
node_tree* first_inorder(node_tree* node) {
    node_tree* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

/* ─── delete_node ─────────────────────────────────────────────────────────────
 * Removes the node containing `word` from the BST, handling 3 cases:
 *   1. Node has no left child  → replace with right child
 *   2. Node has no right child → replace with left child
 *   3. Node has two children   → replace word with in-order successor's word,
 *                                 then delete the successor from the right subtree
 * Returns the (possibly new) root of the subtree.
 */
node_tree* delete_node(node_tree* root, const char* word) {
    if (root == NULL) return root;

    int cmp = strcmp(word, root->word);

    if (cmp < 0) {
        root->left = delete_node(root->left, word);
    } else if (cmp > 0) {
        root->right = delete_node(root->right, word);
    } else {
        if (root->left == NULL) {
            node_tree* temp = root->right;
            free(root->word);
            free(root);
            return temp;
        } else if (root->right == NULL) {
            node_tree* temp = root->left;
            free(root->word);
            free(root);
            return temp;
        }

        node_tree* temp = first_inorder(root->right);
        free(root->word);
        root->word = strdup(temp->word);
        root->right = delete_node(root->right, temp->word);
    }
    return root;
}

/* ─── inorder_to_string ───────────────────────────────────────────────────────
 * Helper for inorder_traversal.
 * Recursively writes each word into `buffer` in alphabetical order,
 * separated by ", ". Uses `pos` as a cursor into the buffer.
 */
char* inorder_to_string(node_tree* root, char* buffer, int* pos) {
    if (root == NULL) return buffer;
    inorder_to_string(LC(root), buffer, pos);
    if (*pos > 1) {
        buffer[(*pos)++] = ',';
        buffer[(*pos)++] = ' ';
    }
    int len = strlen(root->word);
    memcpy(buffer + *pos, root->word, len);
    *pos += len;
    inorder_to_string(RC(root), buffer, pos);
    return buffer;
}

/* ─── inorder_traversal ───────────────────────────────────────────────────────
 * Prints all words in the BST in alphabetical order as a set:
 *   {word1, word2, word3, ...}
 * Uses a fixed 4096-byte buffer to build the output string.
 */
void inorder_traversal(node_tree* root) {
    char buffer[4096];
    int pos = 1;
    buffer[0] = '{';
    inorder_to_string(root, buffer, &pos);
    buffer[pos++] = '}';
    buffer[pos] = '\0';
    printf("%s\n", buffer);
}

/* ─── search ──────────────────────────────────────────────────────────────────
 * Recursively searches for `word` in the BST.
 * Returns 1 if found, 0 if not found.
 * Runs in O(log n) for balanced trees.
 */
int search(node_tree *R, char *word){
    if (R == NULL || word == NULL) return 0;
    int cmp = strcmp(word, R->word);
    if (cmp == 0) return 1;
    if (cmp > 0) return search(RC(R), word);
    else return search(LC(R), word);
}

/* ─── separate_tree ───────────────────────────────────────────────────────────
 * Tokenizes paragraph string `s` by delimiters (spaces, punctuation, etc.)
 * and inserts each unique word into the BST `*R`.
 * The first token becomes the root of the tree.
 * All subsequent tokens are inserted using insert().
 * Returns 1 on success.
 */
int separate_tree(char *s,node_tree **R){
    const char *delims = " .,;:!?-\n\t";
    int ctr=0,i=0;
    char string[50],*word;
    node_tree *p=NULL;
    s= strtok(s, delims);
    if (s != NULL) {
        allocate_word(&word, strlen(s));
        strcpy(word, s);
        create_tree(R);
        ass_val(*R, word);
        while ((s = strtok(NULL, delims)) != NULL) {
            allocate_word(&word, strlen(s));
            strcpy(word, s);
            
            allocate_node(&p);
            ass_val(p, word);
            insert(*R, p);
        }
    }
    return 1;
}

/* ─── union_para ──────────────────────────────────────────────────────────────
 * Computes the UNION of words in paragraphs A and B: R_U = A ∪ B
 * Strategy:
 *   1. Build the BST from paragraph A (using separate_tree)
 *   2. Tokenize paragraph B and insert each word into the same BST
 *   Since insert() discards duplicates, the result is a true set union.
 * If A is empty, only B is used.
 * Returns 1 on success.
 */
int union_para(char *A,char *B,node_tree **R_U){
    char *word=NULL;
    node_tree *p=NULL;
    const char *delims = " .,;:!?-\n\t";
    if(A[0]!='\0'){ 
        separate_tree(A, R_U); 
        B= strtok(B, delims);
        if(B!=NULL){
            allocate_word(&word, strlen(B));
            strcpy(word, B);
            allocate_node(&p);
            ass_val(p, word);
            insert(*R_U, p);
        }
        while((B = strtok(NULL, delims)) != NULL) {        
            allocate_word(&word, strlen(B));
            strcpy(word, B);
            allocate_node(&p);
            ass_val(p, word);
            insert(*R_U, p);
        }
    }
    else{
        separate_tree(B, R_U);
    }
    return 1;
}

/* ─── intersection ────────────────────────────────────────────────────────────
 * Computes the INTERSECTION of words in paragraphs A and B: R_I = A ∩ B
 * Strategy:
 *   1. Build a BST from paragraph A
 *   2. Tokenize paragraph B; for each word, check if it exists in A's BST
 *   3. If yes, insert it into the result tree R_I (duplicates discarded by insert)
 * Returns 1 if at least one common word found, 0 otherwise.
 * R_I remains NULL if there are no common words.
 */
int intersection(char *A, char *B, node_tree **R_I){
    *R_I = NULL;

    if (A == NULL && B == NULL) return 0;
    if (A == NULL || B == NULL) return 0;

    node_tree *R = NULL;
    char *word = NULL;
    node_tree *p = NULL;
    const char *delims = " .,;:!?-\n\t";

    separate_tree(A, &R);
    B = strtok(B, delims);
    if (B != NULL) {
        while (!search(R, B) && B != NULL) B = strtok(NULL, delims);

        /* No common word found at all */
        if (B == NULL) {
            free_tree(&R);
            return 0;
        }

        allocate_word(&word, strlen(B));
        strcpy(word, B);
        create_tree(R_I);
        ass_val(*R_I, word);

        while ((B = strtok(NULL, delims)) != NULL) {
            if (search(R, B)) {
                allocate_word(&word, strlen(B));
                strcpy(word, B);
                allocate_node(&p);
                ass_val(p, word);
                insert(*R_I, p);
            }
        }
    }

    free_tree(&R);
    return (*R_I != NULL) ? 1 : 0;
}

/* ─── difference ──────────────────────────────────────────────────────────────
 * Computes the ALGEBRAIC DIFFERENCE A \ B (words in A that are NOT in B).
 * Strategy:
 *   1. Build the BST from paragraph A
 *   2. Tokenize paragraph B; for each word found, delete it from the BST
 * What remains in the BST is exactly A \ B.
 * Returns 1 on success.
 */
int difference(char *A, char *B, node_tree **R_D){
    char *word=NULL;
    node_tree *p=NULL;
    const char *delims = " .,;:!?-\n\t";
    if(A[0]!='\0'){ 
        separate_tree(A, R_D); 
        B= strtok(B, delims);
        if(B!=NULL){
            *R_D =  delete_node(*R_D, B);
        }
        while((B = strtok(NULL, delims)) != NULL) {        
            *R_D =  delete_node(*R_D, B);
        }
    }
    return 1;
}

/* ─── get_para ────────────────────────────────────────────────────────────────
 * Reads paragraph number `target` from the text file at path `p`.
 * Paragraphs are separated by blank lines (one or more consecutive '\n').
 * Returns a newly allocated string containing the paragraph text,
 * or NULL if the file cannot be opened or the paragraph number doesn't exist.
 *
 * Note: consecutive blank lines count as only ONE paragraph separator.
 */
char *get_para(const char *p, int target) {
    FILE *f = fopen(p, "r");
    if (!f) {
        printf("Error: file '%s' not found in current folder\n", p);
        return NULL;
    }

    int c, cur = 1, last = '\n';
    int size = 128, len = 0;
    int found = 0;
    int in_blank = 0; /* track if we are already in a blank line sequence */

    char *result = malloc(size);
    if (!result) return NULL;

    while ((c = fgetc(f)) != EOF) {
        if (c == '\r') continue; /* skip Windows carriage returns */

        if (last == '\n' && c == '\n') {
            if (!in_blank) {  /* only count the first blank line of a sequence */
                cur++;
                in_blank = 1;
                if (cur > target) break;
            }
            last = c;
            continue;
        }

        /* reset blank sequence when we hit real content */
        if (c != '\n') in_blank = 0;

        if (cur == target) {
            found = 1;
            if (len + 1 >= size) {
                size *= 2;
                result = realloc(result, size);
                if (!result) return NULL;
            }
            result[len++] = c;
        }

        last = c;
    }

    fclose(f);
    result[len] = '\0';

    if (cur >= target) found = 1;

    if (!found) {
        printf("Error: file only has %d paragraph(s), you asked for %d\n", cur, target);
        free(result);
        return NULL;
    }

    return result;
}

/* ─── to_lowercase ────────────────────────────────────────────────────────────
 * Converts all characters in `str` to lowercase in-place.
 * This ensures word comparisons are case-insensitive
 * (e.g., "Hello" and "hello" are treated as the same word).
 */
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}