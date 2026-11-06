// HashTable.cpp
// Purpose: Implementation of the custom facility ID hash table
//          (separate chaining, custom string hash function).
// Author: Bobby Oliver
//
// Collision definition used in this project: a collision is counted when an
// ACCEPTED insertion goes into a bucket that already has at least one entry.
// Rejected duplicate IDs never add to the count.

#include "HashTable.h"
#include "Utils.h"
#include <sstream>

// Creates an empty table with cap buckets (falls back to the default if
// cap is not a positive number).
HashTable::HashTable(int cap) {
    if (cap <= 0) {
        cap = DEFAULT_HASH_CAPACITY;
    }
    capacity = cap;
    entries = 0;
    collisions = 0;
    buckets.resize(capacity);
}

// Turns an ID into a bucket index from 0 to capacity - 1.
// The key is uppercased first so F004 and f004 land in the same bucket.
// Taking % capacity every step keeps the number from overflowing.
int HashTable::hashFunction(const std::string& key) const {
    std::string upper = toUpper(key);
    unsigned long h = 0;
    for (std::size_t i = 0; i < upper.size(); i++) {
        h = (h * 31 + (unsigned char)upper[i]) % capacity;
    }
    return (int)h;
}

// Inserts a facility. Returns false if the ID is already there.
bool HashTable::insert(const Facility& f) {
    std::string key = toUpper(f.id);
    int index = hashFunction(key);

    // check the chain for a duplicate first (before counting anything)
    for (std::size_t i = 0; i < buckets[index].size(); i++) {
        if (buckets[index][i].id == key) {
            return false;   // duplicate, keep the original entry
        }
    }

    // not a duplicate, so this insertion is accepted
    if (!buckets[index].empty()) {
        collisions++;   // bucket already had something in it
    }

    Facility copy = f;
    copy.id = key;      // always store the uppercase ID
    buckets[index].push_back(copy);
    entries++;
    return true;
}

// Looks only in the bucket the ID hashes to (no scanning of everything).
bool HashTable::find(const std::string& id, Facility& result) const {
    std::string key = toUpper(id);
    int index = hashFunction(key);

    for (std::size_t i = 0; i < buckets[index].size(); i++) {
        if (buckets[index][i].id == key) {
            result = buckets[index][i];
            return true;
        }
    }
    return false;
}

int HashTable::getCapacity() const {
    return capacity;
}

int HashTable::getEntries() const {
    return entries;
}

int HashTable::getCollisions() const {
    return collisions;
}

// Text for the facilities load summary line.
std::string HashTable::diagnostic() const {
    std::ostringstream out;
    out << "hash capacity " << capacity
        << ", entries " << entries
        << ", collisions " << collisions;
    return out.str();
}
