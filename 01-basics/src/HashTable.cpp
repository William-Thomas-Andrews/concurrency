#include "HashTable.hpp"



state::state() {
    ;
}

state::state(int i, int j, int k) {
    this->i = i;
    this->j = j;
    this->k = k;
}

state::~state() {}

bool operator==(const state op1, const state op2) {
    if (op1.i == op2.i and op1.j == op2.j and op1.k == op2.k) return true;
    return false;
}

bool operator!=(const state op1, const state op2) {
    if (op1.i == op2.i and op1.j == op2.j and op1.k == op2.k) return false;
    return true;
}

state& state::operator=(const state& other_state) {
    this->i = other_state.i;
    this->j = other_state.j;
    this->k = other_state.k;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const state& input) {
    out << "[" << input.i << "," << input.j << "," << input.k << "]";
    return out;
}

void print_state(state& s) {
    std::cout << "[" << s.i << "," << s.j << "," << s.k << "]" << std::endl;
}

void print_state(const state& s) {
    std::cout << "[" << s.i << "," << s.j << "," << s.k << "]" << std::endl;
}






node& node::operator=(const node& other_node) {
    this->key = other_node.key;
    this->value = other_node.value;
    this->next = NULL;
    return *this;
}

node::node() {
    ;
}

node::node(const state& key, float value) {
    this->key = key;
    this->value = value;
    this->next = NULL;
}

node::node(const node& input) {
    this->key = input.key;
    this->value = input.value;
    this->next = NULL;
}

node::~node() {}

void print_node(const node* input) {
    if (input == NULL) std::cout << "||-||" << std::endl;
    else {
        std::cout << "||" << input->key << ":" << input->value << "|| ";
        node* ptr = input->next;
        while (ptr != NULL) {
            if (ptr->next == NULL)  std::cout << "||" << ptr->key << ":" << ptr->value << "||"; // if last iteration
            else std::cout << "||" << ptr->key << ":" << ptr->value << "||  ";
            ptr = ptr->next;
        }
        std::cout << std::endl;
    }
}

std::ostream& operator<<(std::ostream& out, const node& input) {
    out << "||" << input.key << ":" << input.value << "||";
    return out;
}





HashTable::HashTable() : size(0), capacity(16) {
    this->array = (node**)malloc(sizeof(node*) * this->capacity);
}

HashTable::HashTable(int cap) : size(0), capacity(cap) {
    // std::cout << "[DEBUG] Init hashtable of size " << capacity << " \n";
    this->array = (node**)malloc(sizeof(node*) * this->capacity);
    //////// [DEBUG]
    std::cout << "[DEBUG] Init hashtable of size " << this->capacity << " \n";
    this->print_table();
}

HashTable::HashTable(HashTable& table) {
    this->copy_from(table);
}

void HashTable::copy_from(HashTable& table) {
    table.print_table();
    this->free_table();
    this->array = (node**)malloc(table.capacity * sizeof(node*));
    this->size = 0;
    this->capacity = table.capacity;
    node* ptr;
    for (int i = 0; i < table.capacity; i++) {
        ptr = table.array[i];
        while (ptr != NULL) {
            this->insert(*ptr);
            // this->print_table();
            ptr = ptr->next;
        }
    }
    this->print_table();
}

void HashTable::free_chain(node* base) {
    if (base == NULL) return;
    node* ptr;
    while (base->next != NULL) {
        if (base->next->next == NULL) {
            delete base->next;
            break;
        }
        ptr = base->next;
        base->next = base->next->next;
        delete ptr;
    }
    delete base;
}

void HashTable::free_table() {
    for (int i = 0; i < this->capacity; i++) this->free_chain(this->array[i]);
}

HashTable::~HashTable() {
    this->free_table();
}

HashTable& HashTable::operator=(HashTable& table) {
    this->free_table();
    this->copy_from(table);
    return *this;
}

int HashTable::hash(const state& key) {
    return (((int) key.i) + ((int) key.j) + ((int) key.k)) % this->capacity;
}

float HashTable::lookup(const state& key) {
    node* ptr = this->array[hash(key)];
    while (ptr->key != key) {
        if (ptr->next == NULL) {
            throw std::out_of_range("Item not found.\n");
        }
        ptr = ptr->next;
    }
    return ptr->value;
}

void HashTable::rehash_to(HashTable& table) {
    node* ptr;
    for (int i = 0; i < this->capacity; i++) {
        ptr = this->array[i];
        while (ptr != NULL) {
            table.insert(*ptr);
            ptr = ptr->next;
        }
    }
}

void HashTable::expand() {
    HashTable temp = HashTable(this->capacity * 2);
    this->rehash_to(temp);
    this->copy_from(temp);;
}

void HashTable::insert(node& input) {
    if (this->size == capacity) this->expand(); // increases hash table size if needed
    int index = hash(input.key);
    node* ptr = this->array[index];
    if (ptr == NULL) {
        this->array[index] = new node(input);
        this->size++;
        return;
    }
    while (ptr != NULL and ptr->next != NULL) ptr = ptr->next;
    ptr->next = new node(input);
}

void HashTable::insert(node&& input) {
    if (this->size == capacity) this->expand(); // increases hash table size if needed
     int index = hash(input.key);
    node* ptr = this->array[index];
    if (ptr == NULL) {
        this->array[index] = new node(input);
        this->size++;
        return;
    }
    while (ptr != NULL and ptr->next != NULL) ptr = ptr->next;
    ptr->next = new node(input);
}

void HashTable::insert(const state& key, const float value) { 
    node input{key, value};
    if (this->size == capacity) this->expand(); // increases hash table size if needed
    int index = hash(input.key);
    node* ptr = this->array[index];
    if (ptr == NULL) {
        this->array[index] = new node(input);
        this->size++;
        return;
    }
    while (ptr != NULL and ptr->next != NULL) ptr = ptr->next;
    ptr->next = new node(input);
}

void HashTable::insert(const state&& key, const float value) { 
    node input{key, value};
    if (this->size == capacity) this->expand(); // increases hash table size if needed
    int index = hash(input.key);
    node* ptr = this->array[index];
    if (ptr == NULL) {
        this->array[index] = new node(input);
        this->size++;
        return;
    }
    while (ptr != NULL and ptr->next != NULL) ptr = ptr->next;
    ptr->next = new node(input);
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

void HashTable::print_chain(const node* head) {
    if (head == NULL) {
        std::cout << "(empty)";
        return;
    }
    const node* ptr = head;
    while (ptr != NULL) {
        std::cout << ptr->key << "(" << ptr->value << ")";
        if (ptr->next != NULL) {
            std::cout << " -> ";
        }
        ptr = ptr->next;
    }
}


void HashTable::print_table() {
    std::cout << "HashTable (capacity = " << this->capacity
              << ", size = " << this->size << ")\n";
    std::cout << "--------------------------------------------------\n";

    for (int i = 0; i < this->capacity; ++i) {
        std::cout << "[ ";
        if (i < 10) std::cout << " ";   // alignment for single-digit indices
        std::cout << i << " ] : ";
        print_chain(this->array[i]);
        std::cout << '\n';
    }

    std::cout << "--------------------------------------------------\n";
}

int HashTable::get_table_size() {
    return this->size;
}