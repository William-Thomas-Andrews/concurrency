#include "HashTable.hpp"



state::state() = default;

state::state(int i, int j, int k) {
    this->i = i;
    this->j = j;
    this->k = k;
}

state::~state() {}

bool operator==(const state op1, const state op2) {
    if (op1.i == op2.i && op1.j == op2.j && op1.k == op2.k) return true;
    return false;
}

bool operator!=(const state op1, const state op2) {
    if (op1.i == op2.i && op1.j == op2.j && op1.k == op2.k) return false;
    return true;
}

state& state::operator=(const state& other_state) {
    this->i = other_state.i;
    this->j = other_state.j;
    this->k = other_state.k;
    return *this;
}

std::string get_string(state& state) {
    return "[" + std::to_string(state.i) + "," + std::to_string(state.j) + "," + std::to_string(state.k) + "]";
}

std::string get_string(const state& state) {
    return "[" + std::to_string(state.i) + "," + std::to_string(state.j) + "," + std::to_string(state.k) + "]";
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
    this->next = nullptr;
    return *this;
}

node::node() {
    ;
}

node::node(const state& key, float value) {
    this->key = key;
    this->value = value;
    this->next = nullptr;
}

node::node(const node& input) {
    this->key = input.key;
    this->value = input.value;
    this->next = nullptr;
}

node::~node() {}

std::string get_string(node& node) {
    return  get_string(node.key) + "(" + std::to_string(node.value) + ")";
}

std::string get_string(const node& node) {
    return  get_string(node.key) + "(" + std::to_string(node.value) + ")";
}

void print_node(const node* input) {
    if (input == nullptr) std::cout << "||-||" << std::endl;
    else {
        std::cout << "||" << input->key << ":" << input->value << "|| ";
        node* ptr = input->next;
        while (ptr != nullptr) {
            if (ptr->next == nullptr)  std::cout << "||" << ptr->key << ":" << ptr->value << "||"; // if last iteration
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





HashTable::HashTable() : num_entries(0), num_items(0), capacity(16) {
    // this->array = (node**)malloc(sizeof(node*) * this->capacity); // [DEPRECATED]
    this->array.resize(this->capacity, nullptr);
}

HashTable::HashTable(int cap) : num_entries(0), num_items(0), capacity(cap) {
    // std::cout << "[DEBUG] Init hashtable of num_entries " << capacity << " \n";
    // this->array = (node**)malloc(sizeof(node*) * this->capacity); // [DEPRECATED]
    this->array.resize(this->capacity, nullptr);
    //////// [DEBUG]
    std::cout << "[DEBUG] Init hashtable of size " << this->capacity << " \n";
    this->print_table();
}

HashTable::HashTable(HashTable& table) {
    this->copy_from(table);
}

void HashTable::copy_from(HashTable& table) {
    this->free_table();
    // this->array = (node**)malloc(table.capacity * sizeof(node*)); // [DEPRECATED]
    this->array.clear();
    this->capacity = table.capacity;
    this->array.resize(this->capacity, nullptr);
    this->num_entries = 0;
    this->num_items = 0;
    node* ptr;
    for (int i = 0; i < table.capacity; i++) {
        ptr = table.array[i];
        while (ptr != nullptr) {
            this->insert(*ptr);
            ptr = ptr->next;
        }
    }
}

void HashTable::free_chain(node* base) {
    if (base == nullptr) return;
    node* ptr;
    while (base->next != nullptr) {
        if (base->next->next == nullptr) {
            delete base->next;
            break;
        }
        ptr = base->next;
        base->next = base->next->next;
        delete ptr;
    }
    delete base;
    base = nullptr;
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

node& HashTable::find_node(const state& key) {
    int index = hash(key);
    node* ptr = this->array[index];
    node* prev;
    if (ptr == nullptr) 
        throw std::runtime_error("[find] Error: key not found.");
    else if (ptr->key == key) {
        return *ptr;
    }
    else if (ptr->next == nullptr)  // if this item not the key but the only item in the list
        throw std::runtime_error("[find] Error: key not found.");
    else if (ptr->next != nullptr) {
        prev = ptr;
        ptr = ptr->next;
        while (ptr != nullptr) {
            if (ptr->key == key) {
                return *ptr;
            }
            ptr = ptr->next;
        }
    }
    throw std::runtime_error("[find] Error: key not found.");
}

bool HashTable::in_table(const state& key) {
    int index = hash(key);
    node* ptr = this->array[index];
    node* prev;
    if (ptr == nullptr) 
        return false;
    else if (ptr->key == key) {
        return true;
    }
    else if (ptr->next == nullptr)  // if this item not the key but the only item in the list
        return false;
    else if (ptr->next != nullptr) {
        prev = ptr;
        ptr = ptr->next;
        while (ptr != nullptr) {
            if (ptr->key == key) {
                return true;
            }
            ptr = ptr->next;
        }
    }
    return false;
}

float HashTable::find_val(const state& key) {
    return this->find_node(key).value;
}

void HashTable::rehash_to(HashTable& table) {
    node* ptr;
    for (int i = 0; i < this->capacity; i++) {
        ptr = this->array[i];
        while (ptr != nullptr) {
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
    if (this->in_table(input.key)) 
        throw std::runtime_error("[insert] Error: duplicate key found: " + get_string(input));
    if (this->num_entries == capacity) this->expand(); // increases hash table size if needed
    int index = hash(input.key);
    node* ptr = this->array[index];
    this->num_items++;
    if (ptr == nullptr) {
        this->array[index] = new node(input);
        this->num_entries++;
        return;
    }
    while (ptr != nullptr && ptr->next != nullptr) ptr = ptr->next;
    ptr->next = new node(input);
}

void HashTable::insert(node&& input) {
    if (this->in_table(input.key)) 
        throw std::runtime_error("[insert] Error: duplicate key found: " + get_string(input));
    if (this->num_entries == capacity) this->expand(); // increases hash table size if needed
    int index = hash(input.key);
    node* ptr = this->array[index];
    this->num_items++;
    if (ptr == nullptr) {
        this->array[index] = new node(input);
        this->num_entries++;
        return;
    }
    while (ptr != nullptr && ptr->next != nullptr) ptr = ptr->next;
    ptr->next = new node(input);
}

void HashTable::remove(const state& key) {
    int index = hash(key);
    node* ptr = this->array[index];
    node* prev;
    if (ptr == nullptr) 
        throw std::runtime_error("[remove] Error: key: "+ get_string(key) + " not found.");
    else if (ptr->key == key) {
        this->array[index] = ptr->next;
        free(ptr);
        return;
    }
    else if (ptr->next == nullptr)  // if this item not the key but the only item in the list
        throw std::runtime_error("[remove] Error: key not found.");
    else if (ptr->next != nullptr) {
        prev = ptr;
        ptr = ptr->next;
        while (ptr != nullptr) {
            if (ptr->key == key) {
                prev->next = ptr->next;
                free(ptr);
                return;
            }
            ptr = ptr->next;
        }
    }
    throw std::runtime_error("[remove] Error: key not found.");
}

void HashTable::print_chain(const node* head) {
    if (head == nullptr) {
        std::cout << "(empty)";
        return;
    }
    const node* ptr = head;
    while (ptr != nullptr) {
        std::cout << ptr->key << "(" << ptr->value << ")";
        if (ptr->next != nullptr) {
            std::cout << " -> ";
        }
        ptr = ptr->next;
    }
}


void HashTable::print_table() {
    std::cout << "HashTable (capacity = " << this->capacity
              << ", num_entries = " << this->num_entries << ", num_items = " << this->num_items << ")\n";
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

int HashTable::get_num_entries() {
    return this->num_entries;
}