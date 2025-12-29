#include <iostream>
#include <cstdint>
#include <vector>
#include <shared_mutex>


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

struct Bucket {
    node* head;
    std::shared_mutex lock;
    Bucket(node* node); // 1. Normal Constructor
    ~Bucket(); // 2. Destructor
    Bucket(const Bucket& other); // 3. Copy Constructor
    Bucket& operator=(const Bucket& other); // 4. Copy Assignment Operator
    Bucket(Bucket&& other) noexcept; // 5. Move Constructor
    Bucket& operator=(Bucket&& other) noexcept;// 6. Move Assignment Operator
    void print_bucket() const;
    void free_bucket();
};

std::string get_string(node& input);
std::string get_string(const node& input);
bool operator==(const state& op1, const state& op2);
bool operator!=(const state& op1, const state& op2);
std::ostream& operator<<(std::ostream& out, const node& input);
void print_node(const node* input);

class HashTable {
    private:
        std::vector<Bucket> array;
        int num_entries;
        int num_items;
        int capacity;
        
    public:
        HashTable();
        HashTable(int capacity);
        HashTable(HashTable& table);
        void copy_from(HashTable& table);
        void free_table();
        ~HashTable();
        HashTable& operator=(HashTable& table);
        unsigned int hash(const state& key) const;
        node& find_node(const state& key) const;
        bool in_table(const state& key) const;
        bool in_table(const state& key, int index) const;
        int find_val(const state& key) const;
        void rehash_to(HashTable& table);
        void expand();
        void insert(node& input);
        void insert(node&& input);
        void remove(const state& key);
        void print_table() const;
        int get_num_entries() const;
};

void print_chain(const node* head);
void free_chain(node* base);
