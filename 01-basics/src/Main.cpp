#include "HashTable.hpp"

int main() {
    HashTable table = HashTable();
    state* s = new state(1,2,3);
    print_state(*s);
    node* input = new node(*s, 12.0f);
    table.insert(*input);
    table.print_table();
}