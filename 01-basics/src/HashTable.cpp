#include "HashTable.hpp"



// ------------------------------------------------------------------
// 'state' implementation:

bool operator==(const state& op1, const state& op2) {
    if (op1.i == op2.i && op1.j == op2.j && op1.k == op2.k) return true;
    return false;
}

bool operator!=(const state& op1, const state& op2) {
    if (op1.i == op2.i && op1.j == op2.j && op1.k == op2.k) return false;
    return true;
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

// ------------------------------------------------------------------






// ------------------------------------------------------------------
// 'node' implementation

std::string get_string(node& node) {
    return  get_string(node.key) + "(" + std::to_string(node.value) + ")";
}

std::string get_string(const node& node) {
    return  get_string(node.key) + "(" + std::to_string(node.value) + ")";
}

void print_node(const std::unique_ptr<node> input) {
    if (input == nullptr) std::cout << "||-||" << std::endl;
    else {
        std::cout << "||" << input->key << ":" << input->value << "|| ";
        node* ptr = input->next.get();
        while (ptr != nullptr) {
            if (ptr->next == nullptr)  std::cout << "||" << ptr->key << ":" << ptr->value << "||"; // if last iteration
            else std::cout << "||" << ptr->key << ":" << ptr->value << "||  ";
            ptr = ptr->next.get();
        }
        std::cout << std::endl;
    }
}

std::ostream& operator<<(std::ostream& out, const node& input) {
    out << "||" << input.key << ":" << input.value << "||";
    return out;
}

// ------------------------------------------------------------------





// ------------------------------------------------------------------
// 'Bucket' implementation

// 1. Normal Constructor
Bucket::Bucket(std::unique_ptr<node> other) {
    head = std::move(other);
}

// 2. Destructor
Bucket::~Bucket() {
    free_bucket();
}

// 3. Copy Constructor
Bucket::Bucket(const Bucket& other) {
    std::cout << "here" << std::endl;
    free_bucket();
    node* ptr = other.head.get();
    std::cout << "here" << std::endl;
    if (ptr == nullptr) return;
    head = std::make_unique<node>(*ptr);
    node* cur = head.get();
    while (ptr != nullptr) {
        cur->next = std::make_unique<node>(*ptr);
        cur = cur->next.get();
        ptr = ptr->next.get();
    }
}

// 4. Copy Assignment Operator
Bucket& Bucket::operator=(const Bucket& other) {
    free_bucket();
    node* ptr = other.head.get();
    if (ptr == nullptr) return *this;
    head = std::make_unique<node>(*ptr);
    node* cur = head.get();
    while (ptr != nullptr) {
        cur->next = std::make_unique<node>(*ptr);
        cur = cur->next.get();
        ptr = ptr->next.get();
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
    return *this;
}

void Bucket::print_bucket() const {
    print_chain(head.get());
}

void Bucket::free_bucket() {
    free_chain(std::move(head)); // automatically frees everything from ownership transfer and going out of scope
}

bool Bucket::empty() const noexcept {
    return head == nullptr;
}

int Bucket::insert(node& input) {
    if (head == nullptr) {
        head = std::make_unique<node>(input);
        return 1; // signifies a new entry was added
    }
    if (head->key == input.key) throw std::runtime_error("[insert] Error: duplicate key found: " + get_string(input));
    node* ptr = head.get();
    while (ptr->next != nullptr) {
        if (ptr->key == input.key) throw std::runtime_error("[insert] Error: duplicate key found: " + get_string(input));
        ptr = ptr->next.get();
    }
    ptr->next = std::make_unique<node>(input);
    return 0; // signifies no new entry was added
}

// ------------------------------------------------------------------





// ------------------------------------------------------------------
// 'HashTable' implementation

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
    copy_from(other);
    return *this;
}

// 5. Move Constructor
HashTable::HashTable(HashTable&& other) noexcept {
    num_entries = other.num_entries;
    num_items = other.num_items;
    capacity = other.capacity;
    array = std::move(other.array);
}

// 6. Move Assignment Operator
HashTable& HashTable::operator=(HashTable&& other) noexcept {
    num_entries = other.num_entries;
    num_items = other.num_items;
    capacity = other.capacity;
    array = std::move(other.array);
    return *this;
}

void HashTable::copy_from(const HashTable& other) {
    free_table();
    capacity = other.capacity;
    array.resize(capacity);
    for (int i = 0; i < other.capacity; i++)
        array[i] = other.array[i]; // Bucket copy assignment
}

void HashTable::free_table() {
    array.clear();
    num_entries = 0;
    num_items = 0;
}

unsigned int HashTable::hash(const state& key) const {
    unsigned h = 0;
    h = h * 101 + (unsigned) key.i;
    h = h * 101 + (unsigned) key.j;
    h = h * 101 + (unsigned) key.k;
    return h % capacity;
}

node& HashTable::find_node(const state& key) const {
    if (num_entries == 0) throw std::runtime_error("[find_node] Error: key: "+ get_string(key) + " not found. Errno 1");
    unsigned index = hash(key);
    node* ptr = array[index].head.get();
    if (ptr == nullptr) throw std::runtime_error("[find_node] Error: key: " + get_string(key) + " not found. Errno 2");
    else if (ptr->key == key) return *ptr;
    else if (ptr->next == nullptr) throw std::runtime_error("[find_node] Error: key: " + get_string(key) + " not found. Errno 3");
    else if (ptr->next != nullptr) {
        ptr = ptr->next.get();
        while (ptr != nullptr) {
            if (ptr->key == key)
                return *ptr;
            ptr = ptr->next.get();
        }
    }
    throw std::runtime_error("[find_node] Error: key: "+ get_string(key) + " not found. Errno 4");
}

bool HashTable::in_table(const state& key) const {
    int index = hash(key);
    node* ptr = array[index].head.get();
    if (ptr == nullptr)  return false;
    else if (ptr->key == key) return true;
    else if (ptr->next == nullptr) return false; // if this item not the key but the only item in the list
    else if (ptr->next != nullptr) {
        ptr = ptr->next.get();
        while (ptr != nullptr) {
            if (ptr->key == key)
                return true;
            ptr = ptr->next.get();
        }
    }
    return false;
}

bool HashTable::in_table(const state& key, int index) const {
    node* ptr = array[index].head.get();
    if (ptr == nullptr)  return false;
    else if (ptr->key == key) return true;
    else if (ptr->next == nullptr) return false; // if this item not the key but the only item in the list
    else if (ptr->next != nullptr) {
        ptr = ptr->next.get();
        while (ptr != nullptr) {
            if (ptr->key == key)
                return true;
            ptr = ptr->next.get();
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
        ptr = array[i].head.get();
        while (ptr != nullptr) {
            table.insert(*ptr);
            ptr = ptr->next.get();
        }
    }
}

void HashTable::expand() {
    HashTable temp = HashTable(capacity * 2);
    rehash_to(temp);
    copy_from(temp);;
}

void HashTable::insert(node& input) {
    // std::unique_lock<std::shared_mutex> lock(array[index].lock);
    if (num_entries == capacity) {
        expand(); // increases hash table size
    }
    unsigned index = hash(input.key);
    if (array[index].insert(input) == 1) num_entries++; 
    num_items++;
    // lock automatically unlocks when going out of scope
}

void HashTable::insert(node&& input) {
    // std::unique_lock<std::shared_mutex> lock(array[index].lock);
    if (num_entries == capacity) {
        expand(); // increases hash table size
    }
    unsigned index = hash(input.key);
    if (array[index].insert(input) == 1) num_entries++;
    num_items++;
    // lock automatically unlocks when going out of scope
}

void HashTable::remove(const state& key) {
    int index = hash(key);
    node* ptr = array[index].head.get();
    node* prev;
    if (ptr == nullptr) 
        throw std::runtime_error("[remove] Error: key: " + get_string(key) + " not found. Errno 1");
    else if (ptr->key == key) {
        array[index].head = std::move(ptr->next);
        return;
    }
    else if (ptr->next == nullptr)  // if this item not the key but the only item in the list
        throw std::runtime_error("[remove] Error: key: " + get_string(key) + " not found. Errno 2");
    else if (ptr->next != nullptr) {
        prev = ptr;
        ptr = ptr->next.get();
        while (ptr != nullptr) {
            if (ptr->key == key) {
                prev->next = std::move(ptr->next);
                return;
            }
            ptr = ptr->next.get();
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




// --------------------------------------------------
// Utilities

void print_chain(node* head) {
    if (head == nullptr) {
        std::cout << "(empty)";
        return;
    }
    node* ptr = head;
    while (ptr != nullptr) {
        std::cout << ptr->key << "(" << ptr->value << ")";
        if (ptr->next != nullptr) {
            std::cout << " -> ";
        }
        ptr = ptr->next.get();
    }
}

void free_chain(std::unique_ptr<node> base) {
    // automatically frees
}