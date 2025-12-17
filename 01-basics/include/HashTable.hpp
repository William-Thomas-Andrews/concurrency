#include <iostream>
#include <cstdint>

#pragma once

struct state {
    uint32_t i, j, k;
};

struct node {
    state key;
    float value;
    struct node* next;
};


class HashTable {
    private:
        node* array;
        int size;
        int capacity;
    public:
        HashTable();
        HashTable(HashTable& table);
        ~HashTable();
        float lookup(state key);
        void insert(node input);
        void insert(state key, float value);
        bool remove(state key);
};