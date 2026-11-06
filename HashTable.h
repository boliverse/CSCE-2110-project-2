// HashTable.h
// Purpose: Custom hash table keyed by facility ID. Uses my own string hash
//          function and separate chaining to handle collisions.
//          (No std::unordered_map, std::map or std::hash is used.)
// Author: Bobby Oliver

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include "Facility.h"

// Starting number of buckets. Change it here to test different capacities.
const int DEFAULT_HASH_CAPACITY = 11;

class HashTable {
private:
    // Each bucket is a chain (vector) of facilities. A chain can keep
    // growing, so there is no fixed maximum number of facilities.
    std::vector< std::vector<Facility> > buckets;
    int capacity;     // number of buckets
    int entries;      // number of facilities stored
    int collisions;   // inserts that landed in a bucket that already had items

public:
    // Makes a table with the given number of buckets.
    HashTable(int cap = DEFAULT_HASH_CAPACITY);

    // Custom string hash. The key is made uppercase first, then:
    //   h = 0;  for each character c:  h = (h * 31 + c) % capacity
    int hashFunction(const std::string& key) const;

    // Adds a facility. Returns false (and changes nothing) if a facility
    // with the same ID (ignoring case) is already in the table.
    bool insert(const Facility& f);

    // Looks up an ID (case-insensitive) by checking only its bucket.
    // Returns true and fills result if found, otherwise returns false.
    bool find(const std::string& id, Facility& result) const;

    int getCapacity() const;
    int getEntries() const;
    int getCollisions() const;

    // Returns text like "hash capacity 11, entries 10, collisions 4".
    std::string diagnostic() const;
};

#endif
