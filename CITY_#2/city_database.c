#include <stdio.h>
#include <stdlib.h>

int compare_string(const char *s1, const char *s2) { // Функция сравнения строк
    for (int i = 0; s1[i] != '\0' && s2[i] != '\0'; ++i) {
        if (s1[i] != s2[i]) return 1;
        if (s1[i] == '\0' && s2[i] != '\0') return 1;
        if (s2[i] == '\0' && s1[i] != '\0') return 1;
    }
    return 0;
}

const int sizes[] = {5, 11, 24, 47, 97, 193, 389, 769, 1543, 3072, 3079, 12289, 24539, 49157, 98317, 196613, 393241,
                     786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319};

typedef struct HT_Items_ {
    char *key;
    int data;
    struct HT_Items_ *next;
} HT_Items;

typedef struct HT_ {
    int sizes_index;
    int count;
    HT_Items **array_values;
} HT;

int getHash(char *key) {
    int hash = 0;
    for (int i = 0; key[i] != '\0'; ++i) {
        hash = (hash << 5) - hash + key[i];
    }
    return abs(hash);
}

int getIndex(char *key, HT *ht) {
    int hash = getHash(key);
    return hash % sizes[ht->sizes_index];
}

HT_Items *new_node(char *key, int values) {
    HT_Items *node = malloc(sizeof(HT_Items));
    int n = 0;
    while (key[n] != '\0') n++;
    node->key = malloc(sizeof(char) * (n + 1));
    for (int i = 0; i <= n; ++i)
        node->key[i] = key[i];
    node->data = values;
    node->next = NULL;
    return node;
}
void add(char *key, int values, HT *ht) {
    int index = getIndex(key, ht);
    ht->count++;
    HT_Items *temp = ht->array_values[index];

    if (ht->array_values[index] == NULL)
        ht->array_values[index] = new_node(key, values);
    else {
        while (compare_string(temp->key, key) && temp->next != NULL) {
            temp = temp->next;
        }
        if (!compare_string(temp->key, key))
            temp->data = values;
        else temp->next = new_node(key, values);
    }
}
void free_ht_item(HT_Items *htItems) {
    if (htItems->next != NULL)
        free_ht_item(htItems->next);
    free(htItems);
}

void free_ht(HT *ht) {
    for (int i = 0; i < sizes[ht->sizes_index]; ++i)
        if (ht->array_values[i] != NULL)
            free_ht_item(ht->array_values[i]);
    free(ht->array_values);
    free(ht);
}

void memory_add(HT *ht) {
    ht->sizes_index++;
    ht->count = 0;
    HT_Items **temp_arr = ht->array_values;
    HT_Items *temp = NULL;
    ht->array_values = NULL;
    ht->array_values = malloc(sizeof(HT_Items*) * sizes[ht->sizes_index]);
    for (int i = 0; i < sizes[ht->sizes_index]; ++i)
        ht->array_values[i] = NULL;
    for (int i = 0; i < sizes[ht->sizes_index - 1]; ++i) {
        if (temp_arr[i] != NULL) {
            temp = temp_arr[i];
            add(temp->key, temp->data, ht);
            while (temp->next != NULL) {
                temp = temp->next;
                add(temp->key, temp->data, ht);
            }
            free_ht_item(temp_arr[i]);
        }
    }
    free(temp_arr);
}

HT *init_HT() {
    HT *ht = malloc(sizeof(HT));
    if (!ht) {
        puts("Allocation error!");
        exit(1);
    }
    ht->count = 0;
    ht->sizes_index = 0;
    ht->array_values = malloc(sizeof(HT_Items*) * sizes[ht->sizes_index]);
    if (!ht->array_values) {
        puts("Allocation error!");
        exit(1);
    }
    for (int i = 0; i < sizes[ht->sizes_index]; ++i)
        ht->array_values[i] = NULL;
    return ht;
}

int check_memory(HT *ht) {
    double factor = 0.75;
    return (double) ht->count / sizes[ht->sizes_index] >= factor;
}

void addOrUpdate(char *key, int values, HT *ht) {
    if(check_memory(ht)) memory_add(ht);
    int index = getIndex(key, ht);
    ht->count++;
    HT_Items *temp = ht->array_values[index];

    if (ht->array_values[index] == NULL)
        ht->array_values[index] = new_node(key, values);
    else {
        while (compare_string(temp->key, key) && temp->next != NULL) {
            temp = temp->next;
        }
        if (!compare_string(temp->key, key))
            temp->data = values;
        else temp->next = new_node(key, values);
    }
}

void delete(char *key, HT *ht) {
    int index = getIndex(key, ht);
    HT_Items *temp = ht->array_values[index];
    HT_Items *_temp = temp;

    if (ht->array_values[index] == NULL) {
        puts("Element not found!");
        return;
    }
    else if (ht->array_values[index]->next == NULL) {
        if (!compare_string(ht->array_values[index]->key, key)) {
            free(ht->array_values[index]);
            ht->array_values[index] = NULL;
        }
        else puts("Element not found!");
    }
    else {
        while (compare_string(temp->key, key) && temp->next != NULL) {
            _temp = temp;
            temp = temp->next;
        }
        if (!compare_string(temp->key, key)) {
            _temp->next = temp->next;
            free(temp);
        }
        else puts("Element not found!");
    }
}

void print_ht(HT *ht) {
    for (int i = 0; i < sizes[ht->sizes_index]; ++i) {
        if (ht->array_values[i] != NULL) {
            HT_Items *temp = ht->array_values[i];
            printf("%s *-* %d\n", temp->key, temp->data);
            while (temp->next != NULL) {
                temp = temp->next;
                printf("%s *-* %d\n", temp->key, temp->data);
            }
        }
    }
}

int main() {
    HT *ht = init_HT();
    char c, city[200] = "";
    int population = 0;
    puts("Menu:\n1 - add\n2 - remove\n3 - list");
    while ((c = getchar()) == '1' || c == '2' || c == '3' || c == '\n') {
        if (c == '1') {
            scanf("%s %d", city, &population);
            addOrUpdate(city, population, ht);
        } else if (c == '2') {
            scanf("%s", city);
            delete(city, ht);
        } else if (c == '3')
            print_ht(ht);
    }
    putchar(c);
    free_ht(ht);
    return 0;
}
