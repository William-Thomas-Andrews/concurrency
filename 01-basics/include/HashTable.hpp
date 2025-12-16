#pragma once

class HashTable {
    private:
        float* data;
        int size;
        int capacity;
        // make sure this is a table not just an array
    public:
        HashTable();
        HashTable(HashTable& table);
        float get();
        void insert(float item);
}