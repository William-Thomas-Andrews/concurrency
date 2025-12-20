#include <gtest/gtest.h>
#include "HashTable.hpp"

class HashTableInsertTests : public ::testing::Test {
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

TEST_F(HashTableInsertTests, SingleInsert) {
    state s(1, 2, 3);
    node n(s, 42);
    small_table->insert(n);

    EXPECT_NO_THROW(small_table->find_val(s));
    EXPECT_EQ(small_table->find_val(s), 42);
}

TEST_F(HashTableInsertTests, MultipleInsertsDifferentKeys) {
    state s1(1, 2, 3);
    state s2(4, 5, 6);
    state s3(7, 8, 9);

    small_table->insert(node(s1, 10));
    small_table->insert(node(s2, 20));
    small_table->insert(node(s3, 30));

    EXPECT_EQ(small_table->find_val(s1), 10);
    EXPECT_EQ(small_table->find_val(s2), 20);
    EXPECT_EQ(small_table->find_val(s3), 30);
}

TEST_F(HashTableInsertTests, DuplicateKeyBehavior) {
    state s(1, 2, 3);
    node n1(s, 100);
    small_table->insert(n1);

    node n2(s, 200);
    // Your implementation throws on duplicate keys
    EXPECT_THROW(small_table->insert(n2), std::runtime_error);
}

TEST_F(HashTableInsertTests, InsertTriggersExpansion) {
    for (int i = 0; i < 20; ++i) {
        state s(i, i, i);
        small_table->insert(node(s, i));
    }

    // Verify all items still accessible after expansion
    for (int i = 0; i < 20; ++i) {
        state s(i, i, i);
        EXPECT_EQ(small_table->find_val(s), i);
    }
}

TEST_F(HashTableInsertTests, InsertNodeObject) {
    state s(1, 2, 3);
    node n(s, 99);
    small_table->insert(n);

    EXPECT_EQ(small_table->find_val(s), 99);
}

TEST_F(HashTableInsertTests, CollisionHandling) {
    // Create states that hash to same bucket
    // (depends on your hash function)
    state s1(0, 0, 0);
    state s2(4, 0, 0); // With capacity 4, these might collide

    small_table->insert(node(s1, 111));
    small_table->insert(node(s2, 222));

    EXPECT_EQ(small_table->find_val(s1), 111);
    EXPECT_EQ(small_table->find_val(s2), 222);
}
