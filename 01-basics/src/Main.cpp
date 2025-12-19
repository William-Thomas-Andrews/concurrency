#include "HashTable.hpp"

int main() {
    // HashTable table = HashTable();
    // state* s = new state(1,2,3);
    // print_state(*s);
    // node* input = new node(*s, 12.0f);
    // table.insert(*input);
    // table.print_table();

    HashTable table{3};

    state s{1, 2, 3};
    print_state(s);

    node input{s, 12.0f};
    table.insert(input);

    table.print_table();

    node input2{s, 11.0f};
    table.insert(input2);

    table.print_table();

    state s2{12,453, 452};
    print_state(s2);
    node input3{s2, 3};
    table.insert(input3);
    table.print_table();


    table.insert(node(state(12,453, 452), 2));
    table.print_table();

    table.insert(node(state(12325,43253253, 0), 100));
    table.print_table();

    for (int i = 0; i < 100; i++) {
        table.insert(node(state(rand() % 101,453, 452), 100));
        table.print_table();
    }
    
    std::cout << table.get_table_size() << std::endl;

    std::cout << table.lookup(state(46, 453, 452)) << std::endl;;
}

// TODO: implement custom iterator to abstract things further
// TODO: Make sure all functions including removal work
// TODO: Add iterative hashing and test
// TODO: Add concurrency
// TODO: Add more concurrent methods and compare results