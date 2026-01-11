#include <gtest/gtest.h>
#include "HashTable.hpp"
#include <thread>
#include <random>
#include <chrono>

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


void thread_insertion(int num_threads) {
    std::vector<std::thread> threads;
    HashTable table = HashTable();
    for (int i = 0; i < num_threads; i++)
        threads.push_back(std::thread([&](){
            std::vector<node> inserted;
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::mt19937 engine(seed);
            std::uniform_int_distribution<int> dist(1, 100000000);
            int max = dist(engine);
            for (int j = 0; j < 100; j++) {
                try
                {
                    node new_node = node(state(dist(engine), dist(engine), dist(engine)), dist(engine));
                    table.insert(new_node);
                    inserted.push_back(new_node);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
            for (int k = 0; k < inserted.size(); k++) {
                table.remove(inserted[k].key);
            }
        }));
    for (auto& thread : threads)
        thread.join();
}


TEST_F(HashTableConcurrentTests, TwoThreadInsertion) {
    thread_insertion(2);
}


TEST_F(HashTableConcurrentTests, ManyThreadInsertion) {
    thread_insertion(1000);
}