#include <iostream>
#include <cstdint>

#pragma once

struct state {
    float i, j, k;
};

struct node {
    const state key;
    float value;
    struct node* next;
};

bool operator==(const state op1, const state op2);
bool operator!=(const state op1, const state op2);

class HashTable {
    private:
        node** array;
        int size;
        int capacity;
    public:
        HashTable();
        HashTable(HashTable& table);
        ~HashTable();
        int hash(const state& key);
        float lookup(const state& key);
        void insert(const node& input);
        void insert(const state& key, float value);
        bool remove(const state& key);
};