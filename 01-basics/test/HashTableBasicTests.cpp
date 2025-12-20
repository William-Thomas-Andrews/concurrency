#include <gtest/gtest.h>
#include "HashTable.hpp"

// Basic construction tests
TEST(HashTableBasicTests, DefaultConstructor) {
    ASSERT_NO_THROW(HashTable table);
}

TEST(HashTableBasicTests, CapacityConstructor) {
    HashTable table(32);
    EXPECT_EQ(table.get_num_entries(), 0);
}

TEST(HashTableBasicTests, EmptyTableLookupThrows) {
    HashTable table;
    state s(1, 2, 3);
    EXPECT_THROW(table.find_val(s), std::runtime_error);
}

// State tests
TEST(StateTests, DefaultConstructor) {
    state s;
    // Should compile and not crash
}

TEST(StateTests, ParameterizedConstructor) {
    state s(1, 2, 3);
    EXPECT_EQ(s.i, 1);
    EXPECT_EQ(s.j, 2);
    EXPECT_EQ(s.k, 3);
}

TEST(StateTests, EqualityOperator) {
    state s1(1, 2, 3);
    state s2(1, 2, 3);
    state s3(4, 5, 6);
    
    EXPECT_TRUE(s1 == s2);
    EXPECT_FALSE(s1 == s3);
}

TEST(StateTests, InequalityOperator) {
    state s1(1, 2, 3);
    state s2(4, 5, 6);
    
    EXPECT_TRUE(s1 != s2);
    EXPECT_FALSE(s1 != s1);
}
