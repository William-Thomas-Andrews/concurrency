#include "HashTable.hpp"

bool operator==(const state op1, const state op2) {
    if (op1.i == op2.i and op1.j == op2.j and op1.k == op2.k) return true;
    return false;
}

bool operator!=(const state op1, const state op2) {
    if (op1.i == op2.i and op1.j == op2.j and op1.k == op2.k) return false;
    return true;
}

HashTable::HashTable() : size(0), capacity(16) {
    this->array = (node**)malloc(sizeof(node*) * this->capacity);
}

HashTable::HashTable(HashTable& table) {
    this->size = table.size;
    this->capacity = table.capacity;
    this->array = (node**)malloc(sizeof(node*) * this->capacity);
    for (int i = 0; i < this->capacity; i++) {
        this->array[i] = &node(*table.array[i]); // TODO test if this is a copy or move
        while (this->array[i]->next != NULL) { // creating copies of the chained data too
            this->array[i]->next = &node(*(this->array[i]->next)); // TODO test if this is a copy (it needs to be)
        }
    }
}

int free_chain(node* base) {
    node* ptr;
    while (base->next != NULL) {
        if (base->next->next == NULL) {
            free(base->next);
        }
        ptr = base->next;
        base->next = base->next->next;
        free(ptr);
    }
    free(base);
    return 0;
}

HashTable::~HashTable() {
    for (int i = 0; i < this->capacity; i++) {
        free_chain(this->array[i]);
    }
}

int HashTable::hash(const state& key) {
    return ((int)(key.i * key.j * key.k)) % this->capacity;
}

float HashTable::lookup(const state& key) {
    node* ptr = this->array[hash(key)];
    while (ptr->key != key) {
        ptr = ptr->next;
    }
    return ptr->value;
}

void HashTable::insert(const node& input) {
    int index = hash(input.key);
    node* ptr = this->array[index];
    while (ptr != NULL and ptr->next != NULL) ptr = ptr->next;
    *ptr = input;
}

void HashTable::insert(const state& key, const float value) {
    const node input;
    input.key = key;
    input.value = value;
}

bool HashTable::remove(const state& key) {
    int index = hash(key);
    node* ptr = this->array[index];
    if (ptr == NULL) return false;
    while (ptr->next != NULL) {
        if (ptr->key == key) return true;
        ptr = ptr->next;
    }
    return false;
}