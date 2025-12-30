#include <iostream>
#include <cstdint>
#include <vector>
#include <shared_mutex>


#pragma once

struct state {
    int i, j, k;
    state() = default;
    state(int i, int j, int k) : i(i), j(j), k(k) {}
};

std::string get_string(state& state);
std::string get_string(const state& state);
std::ostream& operator<<(std::ostream& out, const state& input);
void print_state(state& s);
void print_state(const state& s);

struct node {
    state key;
    float value;
    std::unique_ptr<node> next;
    node() = default; // 0. Default Constructor
    node(state k, float v) : key(k), value(v), next(nullptr) {} // 1. Normal Constructor
    node(const node& other) : key(other.key), value(other.value), next(nullptr) {} // 2. Copy Constructor
};

struct Bucket {
    std::unique_ptr<node> head;
    std::shared_mutex lock;
    Bucket() = default;                          // 0. Default Constructor
    Bucket(std::unique_ptr<node> other);         // 1. Normal Constructor
    ~Bucket();                                   // 2. Destructor
    Bucket(const Bucket& other);                 // 3. Copy Constructor
    Bucket& operator=(const Bucket& other);      // 4. Copy Assignment Operator
    Bucket(Bucket&& other) noexcept;            // 5. Move Constructor
    Bucket& operator=(Bucket&& other) noexcept;  // 6. Move Assignment Operator
    void print_bucket() const;
    void free_bucket();
    bool empty();
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
        HashTable(); // 0. Default Constructor
        HashTable(int capacity); // 1. Normal Constructor
        ~HashTable(); // 2. Destructor
        HashTable(const HashTable& other); // 3. Copy Constructor
        HashTable& operator=(const HashTable& other); // 4. Copy Assignment Operator
        HashTable(HashTable&& other) noexcept; // 5. Move Constructor
        HashTable& operator=(HashTable&& other) noexcept; // 6. Move Assignment Operator
        void copy_from(const HashTable& other);
        void free_table();
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

void print_chain(node* head);
void free_chain(std::unique_ptr<node>& base);