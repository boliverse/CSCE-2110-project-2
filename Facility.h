// Facility.h
// Purpose: A Facility record (ID, name, building, category) for the campus
//          facility database.
// Author: Bobby Oliver

#ifndef FACILITY_H
#define FACILITY_H

#include <string>

struct Facility {
    std::string id;        // always stored in uppercase, like F004
    std::string name;      // facility name (names can repeat)
    std::string building;  // spelling comes from buildings.txt
    std::string category;  // category text from the file

    // Default constructor so Facility can be used in vectors and as an
    // "empty" result variable.
    Facility() {}

    // Builds a facility from all four fields.
    Facility(const std::string& i, const std::string& n,
             const std::string& b, const std::string& c) {
        id = i;
        name = n;
        building = b;
        category = c;
    }

    // Returns the facility as one line: ID | Name | Building | Category
    std::string toString() const {
        return id + " | " + name + " | " + building + " | " + category;
    }
};

#endif
