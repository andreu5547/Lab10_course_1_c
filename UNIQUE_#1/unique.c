#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *word;
    int count;
    struct Node *left;
    struct Node *right;
} Node;

struct Node *create_node(char *word){
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->word = (char*)malloc(sizeof(char) * (strlen(word) + 1));
    strcpy(new_node->word, word);
    new_node->count = 1;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

void insert(Node *node, char *word){
    if (strcmp(word, node->word) == 0){
        node->count++;
        return;
    }
    if (strcmp(word, node->word) < 0){
        if (node->left == NULL)
            node->left = create_node(word);
        else insert(node->left, word);
    } else {
        if (node->right == NULL)
            node->right = create_node(word);
        else insert(node->right, word);
    }
}

void print_tree(Node *node, int asc_or_desk) {
    if (node == NULL) {
        return;
    }
    if (asc_or_desk == 0) {
        print_tree(node->left, asc_or_desk);
        printf("%s - %d\n", node->word, node->count);
        print_tree(node->right, asc_or_desk);
    } else if (asc_or_desk == 1) {
        print_tree(node->right, asc_or_desk);
        printf("%s - %d\n", node->word, node->count);
        print_tree(node->left, asc_or_desk);
    } else {
        print_tree(node->left, asc_or_desk);
        printf("%s - %d\n", node->word, node->count);
        print_tree(node->right, asc_or_desk);
    }
}

void check_arg(int argc, char **argv, int *asc_or_desk, FILE **file) {
    if (argc < 2 || argc > 4) {
        puts("Check argument values!");
        exit(1);
    }
    if (argc == 2) {
        *file = fopen(argv[1], "r");
        if (*file == NULL) {
            printf("Could not open file %s\n", argv[1]);
            exit(1);
        }
    }

    if (argc == 4) {
        *file = fopen(argv[3], "r");
        if (*file == NULL) {
            printf("Could not open file %s\n", argv[3]);
            exit(1);
        }
        if (strcmp(argv[2], "ASC") == 0)
            *asc_or_desk = 0;
        else if (strcmp(argv[2], "DESC") == 0)
            *asc_or_desk = 1;
        else {
            printf("Invalid sorting order provided. Please use ASC or DESC.\n");
            exit(1);
        }
    }
}

int main(int argc, char **argv) {
    Node *root = NULL;
    int asc_or_desk = -1;
    FILE *fp = NULL;

    check_arg(argc, argv, &asc_or_desk, &fp);

    char word[100];
    while (fscanf(fp, "%s", word) != EOF) {
        if (root == NULL) {
            root = create_node(word);
        } else {
            insert(root, word);
        }
    }
    fclose(fp);

    print_tree(root, asc_or_desk);
    return 0;
}
