#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STREQ(a, b) (strcmp((a), (b)) == 0)

typedef struct Pair {
    int key, item;
} Pair;

typedef struct HashTable {
    Pair** buckets;  // 1D array of Pair*
    int capacity;  // number of buckets
} HashTable;

HashTable* newTable(int capacity) {
    if (!capacity) return NULL;
    HashTable* table = malloc(sizeof(HashTable));
    table->buckets = calloc(capacity, sizeof(Pair*));
    table->capacity = capacity;
    return table;
}

void freeTable(HashTable* table) {
    if (!table) return;
    if (table->buckets) {
        free(table->buckets); table->buckets = NULL;
    }
    free(table); table = NULL;
}

int divisionHash(int key, int capacity) {
    return key % capacity;
}

int midSquareHash(int key, int numBits) {
    return (key * key) << ((32 - numBits) / 2) >> (32 - numBits);
}

int foldingHash(int key, int param) {
    return 0;
}

int digitAnalysisHash(int key, int param) {
    return 0;
}

void printTable(HashTable* table) {
    printf("hash table (%d):\n", table->capacity);
    for (int i = 0; i < table->capacity; i++) {
        if (table->buckets[i]) {
            printf("%d: (%d, %d)\n", i, table->buckets[i]->key, table->buckets[i]->item);
        }
    }
}

Pair* search(HashTable* table, int key) {
    Pair** ht = table->buckets;
    int home = divisionHash(key, table->capacity);
    int curr = home;
    while (ht[curr] && ht[curr]->key != key) {
        curr = (curr + 1) % table->capacity;    // linear probing
        if (curr == home) {
            printf("search %d -> not found\n", key);
            return NULL;
        }
    }
    if (ht[curr] && ht[curr]->key == key) {
        printf("search %d -> %d found\n", key, ht[curr]->item);
        return ht[curr];
    }
    printf("search %d -> not found\n", key);
    return NULL;
}

void insert(HashTable* table, Pair* pair) {
    Pair** ht = table->buckets;
    int home = divisionHash(pair->key, table->capacity);
    int curr = home;
    while (ht[curr]) {
        curr = (curr + 1) % table->capacity;    // linear probing
        if (curr == home) {
            fprintf(stderr, "hash table is full\n");
            return;
        }
    }
    printf("insert %d -> %d inserted\n", pair->key, pair->item);
    ht[curr] = pair;    // if the inserted key is existed before, then overwrite
}

Pair* delete(HashTable* table, int key) {
    Pair** ht = table->buckets;
    Pair* deleted = NULL;
    int home = divisionHash(key, table->capacity);
    int curr = home;
    while (ht[curr] && ht[curr]->key != key) {
        curr = (curr + 1) % table->capacity;    // linear probing
        if (curr == home) {
            printf("delete %d -> not found\n", key);
            return NULL;
        }
    }
    if (ht[curr] && ht[curr]->key == key) {
        deleted = ht[curr];

        while (ht[curr]) {
            int next = (curr + 1) % table->capacity;    // linear probing
            ht[curr] = ht[next];
            curr = next;
        }
    }
    printf("delete %d -> %d deleted\n", key, deleted->item);
    return deleted;
}

int main(void) {
    HashTable* table = newTable(10);

    insert(table, &(Pair){2, 20});
    insert(table, &(Pair){12, 120});
    insert(table, &(Pair){22, 220});
    insert(table, &(Pair){3, 30});
    insert(table, &(Pair){5, 50});
    insert(table, &(Pair){1, 10});

    search(table, 12);
    search(table, 22);

    delete(table, 12);
    search(table, 12);

    insert(table, &(Pair){12, 120});
    search(table, 12);
    
    insert(table, &(Pair){13, 130});
    search(table, 13);

    freeTable(table);
    return 0;
}
