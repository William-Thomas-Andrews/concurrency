#ifndef HASHTABLE_FIXTURE_HPP
#define HASHTABLE_FIXTURE_HPP

#include <gtest/gtest.h>
#include "HashTable.hpp"

class HashTableFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // Runs before each test
        small_table = new HashTable(4);
        medium_table = new HashTable(16);
    }

    void TearDown() override {
        // Runs after each test
        delete small_table;
        delete medium_table;
    }

    // Helper methods
    void populateTable(HashTable* table, int num_items) {
        for (int i = 0; i < num_items; ++i) {
            state s(i, i * 2, i * 3);
            node n(s, i * 10);
            table->insert(n);
        }
    }

    // Test fixture members
    HashTable* small_table;
    HashTable* medium_table;
};

#endif // HASHTABLE_FIXTURE_HPP
