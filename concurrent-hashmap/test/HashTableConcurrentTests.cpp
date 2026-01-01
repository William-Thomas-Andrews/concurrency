#include <gtest/gtest.h>
#include "HashTable.hpp"

class HashTableConcurrentTests : public ::testing::Test {
protected:
    void SetUp() override {
        small_table = new HashTable(4);
        medium_table = new HashTable(16);
    }

    void TearDown() override {
        delete small_table;
        delete medium_table;
    }

    HashTable* small_table;
    HashTable* medium_table;
};

TEST_F(HashTableConcurrentTests, TwoThreadInsertion) {
    
}