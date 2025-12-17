#include "HashTable.hpp"



state::state() {
    ;
}

state::state(int i, int j, int k) {
    this->i = i;
    this->j = j;
    this->k = k;
}

bool operator==(const state op1, const state op2) {
    if (op1.i == op2.i and op1.j == op2.j and op1.k == op2.k) return true;
    return false;
}

bool operator!=(const state op1, const state op2) {
    if (op1.i == op2.i and op1.j == op2.j and op1.k == op2.k) return false;
    return true;
}

state& state::operator=(const state& other_state) {
    print_state(*this);
    print_state(other_state);
    this->i = other_state.i;
    this->j = other_state.j;
    this->k = other_state.k;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const state& input) {
    out << "<" << input.i << ", " << input.j << ", " << input.k << ">";
    return out;
}

void print_state(state& s) {
    std::cout << "<" << s.i << ", " << s.j << ", " << s.k << ">" << std::endl;
}

void print_state(const state& s) {
    std::cout << "<" << s.i << ", " << s.j << ", " << s.k << ">" << std::endl;
}






node& node::operator=(const node& other_node) {
    this->key = other_node.key;
    this->value = other_node.value;
    return *this;
}

node::node() {
    ;
}

node::node(const state& key, float value) {
    this->key = key;
    this->value = value;
}

node::node(const node& input) {
    this->key = input.key;
    this->value = input.value;
}

void print_node(const node* input) {
    if (input == NULL) std::cout << "||-||";
    else std::cout << "||" << input->key << ":" << input->value << "||";
}

std::ostream& operator<<(std::ostream& out, const node& input) {
    out << "||" << input.key << ":" << input.value << "||";
    return out;
}





HashTable::HashTable() : size(0), capacity(16) {
    this->array = (node**)malloc(sizeof(node*) * this->capacity);
}

HashTable::HashTable(HashTable& table) {
    this->size = table.size;
    this->capacity = table.capacity;
    this->array = (node**)malloc(sizeof(node*) * this->capacity);
    node* ptr;
    for (int i = 0; i < this->capacity; i++) {
        this->array[i] = new node(*table.array[i]); 
        ptr = this->array[i];
        while (ptr->next != NULL) { // creating copies of the chained data too
            this->array[i]->next = new node(*(ptr->next));
            ptr = ptr->next; // move up pointer
        }
    }
}

void HashTable::free_chain(node* base) {
    if (base == NULL) return;
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
}

HashTable::~HashTable() {
    for (int i = 0; i < this->capacity; i++) {
        free_chain(this->array[i]);
    }
}

HashTable& HashTable::operator=(HashTable& table) {
    for (int i = 0; i < this->capacity; i++) {
        free_chain(this->array[i]);
    }
    this->size = table.size;
    this->capacity = table.capacity;
    this->array = (node**)malloc(sizeof(node*) * this->capacity);
    node* ptr;
    for (int i = 0; i < this->capacity; i++) {
        this->array[i] = new node(*table.array[i]); 
        ptr = this->array[i];
        while (ptr->next != NULL) { // creating copies of the chained data too
            this->array[i]->next = new node(*(ptr->next));
            ptr = ptr->next; // move up pointer
        }
    }
    return *this;
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

void HashTable::insert(node& input) {
    int index = hash(input.key);
    node* ptr = this->array[index];
    if (ptr == NULL) {
        std::cout << "heree" << std::endl;
        std::cout << input << std::endl;
        this->array[index] = &input;
        return;
    }
    while (ptr != NULL and ptr->next != NULL) ptr = ptr->next;
    ptr->next = &input;
}

void HashTable::insert(const state& key, const float value) { // TODO: Update this function
    node input;
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

void HashTable::print_table() {
    for (int i = 0; i < this->capacity; i++) {
        // std::cout << *this->array[i] << " ";
        print_node(this->array[i]);
    }
    std::cout << std::endl;
}