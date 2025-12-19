#include <iostream>
#include <cstdint>

#pragma once

struct state {
    float i, j, k;
    state& operator=(const state& other_state);
    state();
    state(int i, int j, int k);
    ~state();
};

std::ostream& operator<<(std::ostream& out, const state& input);

void print_state(state& s);
void print_state(const state& s);

struct node {
    state key;
    float value;
    struct node* next;
    node& operator=(const node& other_node) ;
    node();
    node(const state& key, float value);
    node(const node& input);
    ~node();
};

bool operator==(const state op1, const state op2);
bool operator!=(const state op1, const state op2);
std::ostream& operator<<(std::ostream& out, const node& input);

void print_node(const node* input);

class HashTable {
    private:
        node** array;
        int size;
        int capacity;
        
    public:
        HashTable();
        HashTable(int capacity);
        HashTable(HashTable& table);
        void copy_from(HashTable& table);
        void free_chain(node* base);
        void free_table();
        ~HashTable();
        HashTable& operator=(HashTable& table);
        int hash(const state& key);
        float lookup(const state& key);
        void rehash_to(HashTable& table);
        void expand();
        void insert(node& input);
        void insert(node&& input);
        void insert(const state& key, const float value);
        void insert(const state&& key, const float value);
        bool remove(const state& key);
        void print_chain(const node* head);
        void print_table();
        int get_table_size();
};