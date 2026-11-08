// main.cpp
// Purpose: Program entry point. Loads the data files, then runs the
//          repeating Milestone 1 menu.
// Author: Bobby Oliver
//
// NOTE: Right now the facility options (1 to 4) are built. The graph and
// tree options (5 to 11) print a "not built yet" message until those
// modules are merged in. loadBuildingNames() is a stand-in until the
// CampusGraph module loads buildings.txt itself.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "FacilityDatabase.h"
#include "DepartmentTree.h"
#include "Utils.h"

using namespace std;

// Stand-in loader: reads buildings.txt into a list of names so facilities
// can be checked against them. Rejects blank names and duplicates
// (ignoring case) and prints warnings and a load summary.
void loadBuildingNames(const string& dataDir, vector<string>& names) {
    const string fileName = "buildings.txt";
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
            if (lineNumber == 1 && line.size() >= 3 &&
                line.compare(0, 3, "\xEF\xBB\xBF") == 0) {
                line = line.substr(3);
            }
            if (isBlankOrComment(line)) {
                continue;
            }

            string name = trim(line);
            if (name.find('|') != string::npos) {
                cout << "Warning: " << fileName << " line " << lineNumber
                     << ": building name must not contain '|'" << endl;
                rejected++;
                continue;
            }

            bool duplicate = false;
            for (size_t i = 0; i < names.size(); i++) {
                if (equalsIgnoreCase(names[i], name)) {
                    duplicate = true;
                }
            }
            if (duplicate) {
                cout << "Warning: " << fileName << " line " << lineNumber
                     << ": duplicate building '" << name << "'" << endl;
                rejected++;
            } else {
                names.push_back(name);
                accepted++;
            }
        }
    }
    cout << fileName << ": " << accepted << " accepted, "
         << rejected << " rejected" << endl;
}

// Prints the Milestone 1 menu (numbers from Section 6.9, Exit is 0).
void printMenu() {
    cout << endl;
    cout << "===== Smart Campus Menu =====" << endl;
    cout << "1. View all facilities" << endl;
    cout << "2. Search facility by ID" << endl;
    cout << "3. Search facilities by name" << endl;
    cout << "4. Search facilities by category" << endl;
    cout << "5. View campus graph" << endl;
    cout << "6. Display graph counts and degrees" << endl;
    cout << "7. Find reachable buildings (BFS)" << endl;
    cout << "8. Add department" << endl;
    cout << "9. Search department" << endl;
    cout << "10. Display department hierarchy" << endl;
    cout << "11. Run preorder traversal" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter choice: ";
}

// Reads one whole line and turns it into a menu number.
// Returns -1 for blank or non-numeric input, and 0 (exit) if the input
// stream has ended so we never loop forever.
int readMenuChoice() {
    string line;
    if (!getline(cin, line)) {
        return 0;
    }
    line = trim(line);
    if (line.empty() || line.size() > 3) {
        return -1;
    }
    int value = 0;
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] < '0' || line[i] > '9') {
            return -1;
        }
        value = value * 10 + (line[i] - '0');
    }
    return value;
}

int main(int argc, char* argv[]) {
    // optional command-line argument = data directory (default is the
    // current directory, where all the project files live)
    string dataDir = ".";
    if (argc > 1) {
        dataDir = argv[1];
    }

    // load the files in the required order
    vector<string> buildingNames;
    loadBuildingNames(dataDir, buildingNames);

    FacilityDatabase facilityDb;
    facilityDb.loadFromFile(dataDir, "facilities.txt", buildingNames);

    DepartmentTree departmentTree;
    departmentTree.loadFromFile(dataDir, "departments.txt");

    int choice = -1;
    while (choice != 0) {
        printMenu();
        choice = readMenuChoice();

        if (choice == 1) {
            facilityDb.displayAll();
        } else if (choice == 2) {
            cout << "Enter facility ID: ";
            string typed;
            getline(cin, typed);
            facilityDb.searchById(typed);
        } else if (choice == 3) {
            cout << "Enter facility name: ";
            string typed;
            getline(cin, typed);
            facilityDb.searchByName(typed);
        } else if (choice == 4) {
            cout << "Enter facility category: ";
            string typed;
            getline(cin, typed);
            facilityDb.searchByCategory(typed);
        } else if (choice >= 5 && choice <= 7) {
            cout << "That option is not built yet." << endl;
        } else if (choice == 8) {
            string name;
            string parentName;
            cout << "Enter department name: ";
            getline(cin, name);
            if(departmentTree.isEmpty()) 
            {
                departmentTree.addDepartment(name, "");
            } 
            else 
            {
                cout << "Enter parent department name: ";
                getline(cin, parentName);
                departmentTree.addDepartment(name, parentName);
            }
        } else if (choice == 9) {
            string name;
            cout << "Enter department name to search: ";
            getline(cin, name);
            departmentTree.searchDepartment(name);
        } else if (choice == 10) {
            departmentTree.displayHierarchy();
        } else if (choice == 11) {
            departmentTree.preorderTraversal();
        } else if (choice == 0) {
            cout << "Goodbye." << endl;
        } else {
            cout << "Invalid choice. Please enter a number from the menu." << endl;
        }
    }

    return 0;
}
