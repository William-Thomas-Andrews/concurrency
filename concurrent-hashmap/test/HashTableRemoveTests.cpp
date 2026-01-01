#include <gtest/gtest.h>
#include "HashTable.hpp"

class HashTableRemoveTests : public ::testing::Test {
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

TEST_F(HashTableRemoveTests, RemoveExistingKey) {
    state s(1, 2, 3);
    small_table->insert(node(s, 42));

    EXPECT_NO_THROW(small_table->remove(s));
    EXPECT_THROW(small_table->find_val(s), std::runtime_error);
}

TEST_F(HashTableRemoveTests, RemoveNonExistentKey) {
    state s(1, 2, 3);
    EXPECT_THROW(small_table->remove(s), std::runtime_error);
}

TEST_F(HashTableRemoveTests, RemoveFromEmptyTable) {
    state s(1, 2, 3);
    EXPECT_THROW(small_table->remove(s), std::runtime_error);
}

TEST_F(HashTableRemoveTests, RemoveHeadOfChain) {
    state s1(0, 0, 0);
    state s2(4, 0, 0); // Should collide in small_table

    small_table->insert(node(s1, 100));
    small_table->insert(node(s2, 200));

    EXPECT_NO_THROW(small_table->remove(s1));
    EXPECT_EQ(small_table->find_val(s2), 200);
}

TEST_F(HashTableRemoveTests, RemoveMiddleOfChain) {
    // Create 3 colliding states
    state s1(0, 0, 0);
    state s2(4, 0, 0);
    state s3(8, 0, 0);

    small_table->insert(node(s1, 100));
    small_table->insert(node(s2, 200));
    small_table->insert(node(s3, 300));

    EXPECT_NO_THROW(small_table->remove(s2));
    EXPECT_EQ(small_table->find_val(s1), 100);
    EXPECT_EQ(small_table->find_val(s3), 300);
}

TEST_F(HashTableRemoveTests, RemoveTailOfChain) {
    state s1(0, 0, 0);
    state s2(4, 0, 0);

    small_table->insert(node(s1, 100));
    small_table->insert(node(s2, 200));

    EXPECT_NO_THROW(small_table->remove(s2));
    EXPECT_EQ(small_table->find_val(s1), 100);
}

TEST_F(HashTableRemoveTests, InsertAfterRemove) {
    state s(1, 2, 3);
    small_table->insert(node(s, 42));
    small_table->remove(s);
    small_table->insert(node(s, 84));

    EXPECT_EQ(small_table->find_val(s), 84);
}
