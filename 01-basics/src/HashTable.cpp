#include "HashTable.hpp"



state::state() = default;

state::state(int i, int j, int k) : i(i), j(j), k(k) {}

state::~state() {}

bool operator==(const state& op1, const state& op2) {
    if (op1.i == op2.i && op1.j == op2.j && op1.k == op2.k) return true;
    return false;
}

bool operator!=(const state& op1, const state& op2) {
    if (op1.i == op2.i && op1.j == op2.j && op1.k == op2.k) return false;
    return true;
}

state& state::operator=(const state& other) {
    i = other.i;
    j = other.j;
    k = other.k;
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
    key = other_node.key;
    value = other_node.value;
    next = nullptr;
    return *this;
}

node::node() {
    ;
}

node::node(const state& other_key, int other_value) {
    key = other_key;
    value = other_value;
    next = nullptr;
}

node::node(const node& input) {
    key = input.key;
    value = input.value;
    next = nullptr;
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


// 0. Default Constructor
Bucket::Bucket() {
    head = nullptr;
}

// 1. Normal Constructor
Bucket::Bucket(node* node) {
    head = node;
}

// 2. Destructor
Bucket::~Bucket() {
    free_bucket();
}

// 3. Copy Constructor
Bucket::Bucket(const Bucket& other) {
    free_bucket();
    node* ptr = other.head;
    head = new node(*ptr);
    node* cur = head;
    while (ptr != nullptr) {
        cur->next = new node(*ptr);
        ptr = ptr->next;
    }
}

// 4. Copy Assignment Operator
Bucket& Bucket::operator=(const Bucket& other) {
    free_bucket();
    node* ptr = other.head;
    head = new node(*ptr);
    node* cur = head;
    while (ptr != nullptr) {
        cur->next = new node(*ptr);
        ptr = ptr->next;
    }
    return *this;
}

// 5. Move Constructor
Bucket::Bucket(Bucket&& other) noexcept : head(std::exchange(other.head, nullptr)) {
    // transfers ownership and leaves the source in a valid, empty state
}

// 6. Move Assignment Operator
Bucket& Bucket::operator=(Bucket&& other) noexcept { 
    // transfers ownership and leaves the source in a valid, empty state
    head = std::exchange(other.head, nullptr);
}

void Bucket::print_bucket() const {
    print_chain(head);
}

void Bucket::free_bucket() {
    free_chain(head);
}

bool Bucket::empty() {
    return head == nullptr;
}



// ------------------------------------------------------------------
// HashTable Implementation

// 0. Default Constructor
HashTable::HashTable() : num_entries(0), num_items(0), capacity(16) {
    array.resize(capacity);
}

// 1. Normal Constructor
HashTable::HashTable(int cap) : num_entries(0), num_items(0), capacity(cap) {
    array.resize(capacity);
}

// 2. Destructor
HashTable::~HashTable() {
    free_table();
}

// 3. Copy Constructor
HashTable::HashTable(const HashTable& other) {
    copy_from(other);
}

// 4. Copy Assignment Operator
HashTable& HashTable::operator=(const HashTable& other) {

}

// 5. Move Constructor
HashTable::HashTable(HashTable&& other) noexcept {

}

// 6. Move Assignment Operator
HashTable& HashTable::operator=(HashTable&& other) noexcept {

}

void HashTable::copy_from(HashTable& table) {
    free_table();
    capacity = table.capacity;
    array.resize(capacity);
    node* ptr;
    for (int i = 0; i < table.capacity; i++) {
        ptr = table.array[i].head;
        while (ptr != nullptr) {
            insert(*ptr);
            ptr = ptr->next;
        }
    }
}

void HashTable::free_table() {
    for (int i = 0; i < capacity; i++) array[i].free_bucket(); 
    num_entries = 0;
    num_items = 0;
}

HashTable& HashTable::operator=(HashTable& table) {
    free_table();
    copy_from(table);
    return *this;
}

unsigned int HashTable::hash(const state& key) const {
    unsigned h = 0;
    h = h * 101 + (unsigned) key.i;
    h = h * 101 + (unsigned) key.j;
    h = h * 101 + (unsigned) key.k;
    return h % capacity;
}

node& HashTable::find_node(const state& key) const {
    if (num_entries == 0) 
        throw std::runtime_error("[find_node] Error: key: "+ get_string(key) + " not found. Errno 1");
    unsigned index = hash(key);
    node* ptr = array[index].head;
    if (ptr == nullptr) {
        throw std::runtime_error("[find_node] Error: key: " + get_string(key) + " not found. Errno 2");
    }
    else if (ptr->key == key) {
        return *ptr;
    }
    else if (ptr->next == nullptr)  // if this item not the key but the only item in the list
        throw std::runtime_error("[find_node] Error: key: " + get_string(key) + " not found. Errno 3");
    else if (ptr->next != nullptr) {
        ptr = ptr->next;
        while (ptr != nullptr) {
            if (ptr->key == key)
                return *ptr;
            ptr = ptr->next;
        }
    }
    throw std::runtime_error("[find_node] Error: key: "+ get_string(key) + " not found. Errno 4");
}

bool HashTable::in_table(const state& key) const {
    int index = hash(key);
    node* ptr = array[index].head;
    if (ptr == nullptr) 
        return false;
    else if (ptr->key == key) {
        return true;
    }
    else if (ptr->next == nullptr)  // if this item not the key but the only item in the list
        return false;
    else if (ptr->next != nullptr) {
        ptr = ptr->next;
        while (ptr != nullptr) {
            if (ptr->key == key)
                return true;
            ptr = ptr->next;
        }
    }
    return false;
}

bool HashTable::in_table(const state& key, int index) const {
    std::cout << "yep" << std::endl;
    node* ptr = array[index].head;
    if (ptr == nullptr) 
        return false;
    else if (ptr->key == key) {
        return true;
    }
    else if (ptr->next == nullptr)  // if this item not the key but the only item in the list
        return false;
    else if (ptr->next != nullptr) {
        ptr = ptr->next;
        while (ptr != nullptr) {
            if (ptr->key == key)
                return true;
            ptr = ptr->next;
        }
    }
    return false;
}

int HashTable::find_val(const state& key) const {
    return find_node(key).value;
}

void HashTable::rehash_to(HashTable& table) {
    node* ptr;
    for (int i = 0; i < capacity; i++) {
        ptr = array[i].head;
        while (ptr != nullptr) {
            table.insert(*ptr);
            ptr = ptr->next;
        }
    }
}

void HashTable::expand() {
    HashTable temp = HashTable(capacity * 2);
    rehash_to(temp);
    copy_from(temp);;
}

void HashTable::insert(node& input) {
    unsigned index = hash(input.key);
    // std::unique_lock<std::shared_mutex> lock(array[index].lock);
    if (num_entries == capacity) {
        expand(); // increases hash table size
        index = hash(input.key); // rehash
    }
    node* ptr = array[index].head;
    num_items++;
    if (ptr == nullptr) {
        array[index].head = new node(input);
        num_entries++;
        return;
    }
    if (ptr->key == input.key) 
        throw std::runtime_error("[insert] Error: duplicate key found: " + get_string(input));
    while (ptr != nullptr && ptr->next != nullptr) {
        ptr = ptr->next;
        if (ptr->key == input.key) 
            throw std::runtime_error("[insert] Error: duplicate key found: " + get_string(input));
    }
    ptr->next = new node(input);
    // lock automatically unlocks when going out of scope
}

void HashTable::insert(node&& input) {
    unsigned index = hash(input.key);
    // std::unique_lock<std::shared_mutex> lock(array[index].lock);
    if (num_entries == capacity) {
        expand(); // increases hash table size
        index = hash(input.key); // rehash
    }
    node* ptr = array[index].head;
    num_items++;
    if (ptr == nullptr) {
        array[index].head = new node(input);
        num_entries++;
        return;
    }    
    if (ptr->key == input.key) 
        throw std::runtime_error("[insert] Error: duplicate key found: " + get_string(input));
    while (ptr != nullptr && ptr->next != nullptr) {
        ptr = ptr->next;
        if (ptr->key == input.key) 
            throw std::runtime_error("[insert] Error: duplicate key found: " + get_string(input));
    }
    ptr->next = new node(input);
    // lock automatically unlocks when going out of scope
}

void HashTable::remove(const state& key) {
    int index = hash(key);
    node* ptr = array[index].head;
    node* prev;
    if (ptr == nullptr) 
        throw std::runtime_error("[remove] Error: key: " + get_string(key) + " not found. Errno 1");
    else if (ptr->key == key) {
        array[index].head = ptr->next;
        free(ptr);
        return;
    }
    else if (ptr->next == nullptr)  // if this item not the key but the only item in the list
        throw std::runtime_error("[remove] Error: key: " + get_string(key) + " not found. Errno 2");
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
    throw std::runtime_error("[remove] Error: key: "+ get_string(key) + " not found. Errno 3");
}

void HashTable::print_table() const {
    std::cout << "HashTable (capacity = " << capacity
              << ", num_entries = " << num_entries << ", num_items = " << num_items << ")\n";
    std::cout << "--------------------------------------------------\n";

    for (int i = 0; i < capacity; ++i) {
        std::cout << "[ ";
        if (i < 10) std::cout << " ";   // alignment for single-digit indices
        std::cout << i << " ] : ";
        array[i].print_bucket();
        std::cout << '\n';
    }

    std::cout << "--------------------------------------------------\n";
}

int HashTable::get_num_entries() const {
    return num_entries;
}



void print_chain(const node* head) {
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

void free_chain(node*& base) {
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