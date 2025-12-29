#include <iostream>
#include <cstdint>
#include <vector>

#pragma once

struct state {
    int i, j, k;
    state& operator=(const state& other_state);
    state();
    state(int i, int j, int k);
    ~state();
};

std::string get_string(state& state);
std::string get_string(const state& state);
std::ostream& operator<<(std::ostream& out, const state& input);
void print_state(state& s);
void print_state(const state& s);

struct node {
    state key;
    float value;
    struct node* next;
    node& operator=(const node& other_node);
    node();
    node(const state& key, int value);
    node(const node& input);
    ~node();
};

std::string get_string(node& input);
std::string get_string(const node& input);
bool operator==(const state& op1, const state& op2);
bool operator!=(const state& op1, const state& op2);
std::ostream& operator<<(std::ostream& out, const node& input);
void print_node(const node* input);

class HashTable {
    private:
        std::vector<node*> array;
        int num_entries;
        int num_items;
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
        unsigned int hash(const state& key) const;
        node& find_node(const state& key) const;
        bool in_table(const state& key) const;
        int find_val(const state& key) const;
        void rehash_to(HashTable& table);
        void expand();
        void insert(node& input);
        void insert(node&& input);
        void remove(const state& key);
        void print_chain(const node* head) const;
        void print_table() const;
        int get_num_entries() const;
};