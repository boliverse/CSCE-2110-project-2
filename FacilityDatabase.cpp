// FacilityDatabase.cpp
// Purpose: Implementation of the facility database (file loading with
//          validation, display, and ID search through the hash table).
// Author: Bobby Oliver

#include "FacilityDatabase.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Looks for a building name (ignoring case) in the accepted building list.
// Returns the spelling from the list, or "" if it is not there.
static string findBuilding(const vector<string>& buildings, const string& name) {
    for (size_t i = 0; i < buildings.size(); i++) {
        if (equalsIgnoreCase(buildings[i], name)) {
            return buildings[i];
        }
    }
    return "";
}

FacilityDatabase::FacilityDatabase() {
    // table and vector start out empty
}

// Reads facilities.txt line by line, checks each record, and fills the
// vector and the hash table with the good ones.
void FacilityDatabase::loadFromFile(const string& dataDir, const string& fileName,
                                    const vector<string>& buildings) {
    int accepted = 0;
    int rejected = 0;

    ifstream in(joinPath(dataDir, fileName).c_str());
    if (!in) {
        cout << "Error: could not open " << fileName
             << " - treating it as empty." << endl;
    } else {
        string line;
        int lineNumber = 0;

        while (getline(in, line)) {
            lineNumber++;

            // ignore a UTF-8 byte order mark if the file starts with one
            if (lineNumber == 1 && line.size() >= 3 &&
                line.compare(0, 3, "\xEF\xBB\xBF") == 0) {
                line = line.substr(3);
            }

            if (isBlankOrComment(line)) {
                continue;   // not a record, so not accepted or rejected
            }

            vector<string> fields = splitFields(line);
            string reason = "";
            Facility f;

            if (fields.size() != 4) {
                ostringstream msg;
                msg << "wrong number of fields (expected 4, found "
                    << fields.size() << ")";
                reason = msg.str();
            } else {
                string id = toUpper(trim(fields[0]));   // normalize first
                string name = trim(fields[1]);
                string buildingName = trim(fields[2]);
                string category = trim(fields[3]);

                if (id.empty() || name.empty() || buildingName.empty() || category.empty()) {
                    reason = "empty field";
                } else if (!isValidFacilityId(id)) {
                    reason = "invalid facility ID '" + id +
                             "' (must be F plus exactly three digits)";
                } else {
                    string realBuilding = findBuilding(buildings, buildingName);
                    if (realBuilding.empty()) {
                        reason = "unknown building '" + buildingName + "'";
                    } else {
                        f = Facility(id, name, realBuilding, category);
                        // insert fails if the ID is already in the table
                        if (!table.insert(f)) {
                            reason = "duplicate facility ID " + id +
                                     " (keeping the first record)";
                        }
                    }
                }
            }

            if (reason.empty()) {
                facilities.push_back(f);
                accepted++;
            } else {
                cout << "Warning: " << fileName << " line " << lineNumber
                     << ": " << reason << endl;
                rejected++;
            }
        }
    }

    cout << fileName << ": " << accepted << " accepted, "
         << rejected << " rejected (" << table.diagnostic() << ")" << endl;
}

// Shows all facilities in the order they were loaded.
void FacilityDatabase::displayAll() const {
    if (facilities.empty()) {
        cout << "No facilities are loaded." << endl;
        return;
    }
    for (size_t i = 0; i < facilities.size(); i++) {
        cout << facilities[i].toString() << endl;
    }
    cout << "Total facilities: " << facilities.size() << endl;
}

// Searches by ID using only the hash table.
void FacilityDatabase::searchById(const string& typedId) const {
    string id = toUpper(trim(typedId));   // normalize before anything else

    if (!isValidFacilityId(id)) {
        cout << "Invalid format: an ID is F followed by exactly three digits." << endl;
        return;
    }

    Facility found;
    if (table.find(id, found)) {
        cout << "Found: " << found.name << " | " << found.building
             << " | " << found.category << endl;
    } else {
        cout << "Not found: no facility with ID " << id << endl;
    }
}

int FacilityDatabase::getCount() const {
    return (int)facilities.size();
}
