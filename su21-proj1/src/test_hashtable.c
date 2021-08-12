#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>

unsigned int hF(void* one) {
    return 4;
}

int eF(void* one, void* two) {
    return *((int*) one) == *((int*) two);
}

int main(int argc, char **argv) {
    HashTable *ht = createHashTable(10, hF, eF);
    int key1 = 1;
    int data1 = 1;
    int key2 = 2;
    int data2 = 2;
    int key3 = 3;
    insertData(ht, &key1, &data1);
    insertData(ht, &key2, &data2);
    printf("%d\n", *((int*) findData(ht, &key1)));
    printf("%d\n", *((int*) findData(ht, &key2)));

    for (int i = 1; i < 10; ++i) {
        int k = i + 5;
        insertData(ht, &i, &k);
        printf("%d\n", *((int*) findData(ht, &i)));
    }
    return 0;
}