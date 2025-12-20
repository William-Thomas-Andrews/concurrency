#include <gtest/gtest.h>
#include "HashTable.hpp"

class HashTableEdgeCaseTests : public ::testing::Test {
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

TEST_F(HashTableEdgeCaseTests, LargeScaleInsert) {
    for (int i = 0; i < 1000; ++i) {
        state s(i, i * 2, i * 3);
        medium_table->insert(node(s, i));
    }

    // Spot check some values
    state s_mid(500, 1000, 1500);
    EXPECT_EQ(medium_table->find_val(s_mid), 500);
}

TEST_F(HashTableEdgeCaseTests, CopyConstructor) {
    state s1(1, 2, 3);
    state s2(4, 5, 6);
    small_table->insert(node(s1, 10));
    small_table->insert(node(s2, 20));

    HashTable copy(*small_table);

    EXPECT_EQ(copy.find_val(s1), 10);
    EXPECT_EQ(copy.find_val(s2), 20);
}

TEST_F(HashTableEdgeCaseTests, AssignmentOperator) {
    state s1(1, 2, 3);
    small_table->insert(node(s1, 42));

    HashTable assigned(8);
    assigned = *small_table;

    EXPECT_EQ(assigned.find_val(s1), 42);
}

TEST_F(HashTableEdgeCaseTests, NegativeValues) {
    state s(-1, -2, -3);
    small_table->insert(node(s, -99));    

    EXPECT_EQ(small_table->find_val(s), -99);
}

TEST_F(HashTableEdgeCaseTests, ZeroValues) {
    state s(0, 0, 0);
    small_table->insert(node(s, 0));

    EXPECT_EQ(small_table->find_val(s), 0);
}

TEST_F(HashTableEdgeCaseTests, FloatPrecision) {
    state s(1, 2, 3);
    small_table->insert(node(s, 3));

    EXPECT_EQ(small_table->find_val(s), 3);
}

// Parameterized test example
class HashTableParameterizedTests :
    public ::testing::Test,
    public ::testing::WithParamInterface<int> {
protected:
    void SetUp() override {
        // Setup done per test
    }
    void TearDown() override {
        // Cleanup done per test
    }
};

TEST_P(HashTableParameterizedTests, VariousCapacities) {
    int capacity = GetParam();
    HashTable table(capacity);

    state s(1, 2, 3);
    table.insert(node(s, 100));

    EXPECT_EQ(table.find_val(s), 100);
}

INSTANTIATE_TEST_SUITE_P(
    CapacityTests,
    HashTableParameterizedTests,
    ::testing::Values(1, 2, 4, 8, 16, 32, 64, 128)
);
