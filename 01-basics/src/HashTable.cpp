#include "HashTable.hpp"

HashTable::HashTable() : size(0), capacity(16) {
    this->array = (node*)malloc(sizeof(node) * this->capacity);
}

HashTable::HashTable(HashTable& table) {
    this->size = table.size;
    this->capacity = table.capacity;
    this->array = (node*)malloc(sizeof(node) * this->capacity);
    for (int i = 0; i < this->capacity; i++) {
        this->array[i] = node(table.array[i]); // TODO test if this is a copy or move
        while (this->array[i].next != NULL) { // creating copies of the chained data too
            this->array[i].next = &node(*(this->array[i].next)); // TODO test if this is a copy (it needs to be)
        }
    }
}

int free_chain(node* base) {
    while (base->next != NULL) {
        ;
    }
}

HashTable::~HashTable() {

}

