// FacilityDatabase.h
// Purpose: Holds all accepted facilities (in file order) plus the custom
//          ID hash table. Loads facilities.txt, displays facilities, and
//          searches by ID using the hash table.
// Author: Bobby Oliver

#ifndef FACILITYDATABASE_H
#define FACILITYDATABASE_H

#include <string>
#include <vector>
#include "Facility.h"
#include "HashTable.h"

class FacilityDatabase {
private:
    std::vector<Facility> facilities;   // accepted facilities in file order
    HashTable table;                    // custom hash table keyed by ID

public:
    FacilityDatabase();

    // Loads facilities.txt from dataDir. buildings is the list of accepted
    // building names (spelling from buildings.txt) used to check each
    // record's building. Prints warnings and one load summary line.
    void loadFromFile(const std::string& dataDir, const std::string& fileName,
                      const std::vector<std::string>& buildings);

    // Prints every facility in file order and the total.
    void displayAll() const;

    // Trims and uppercases the typed ID, then prints one of three results:
    // invalid format, valid but not found, or found.
    void searchById(const std::string& typedId) const;

    int getCount() const;
};

#endif
