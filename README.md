# CSCE 2110 Project 2
Smart Campus Navigation and Facility Analytics System

Author: Bobby Oliver
Updated: November 9, 2026


## Purpose

This program loads campus data from plain text files and gives the user a menu
for working with it. At startup it reads buildings.txt, connections.txt,
facilities.txt and departments.txt, then lets the user list every facility,
look one up by ID, search by facility name or category, walk the campus graph,
and inspect the department hierarchy.

Facilities are kept in a vector for listing and in a hand-written hash table
for ID lookup. The campus map is an undirected graph stored as adjacency
lists. The department hierarchy is a general tree, with one root department
and a list of children at each node.

No standard container from the map family is used anywhere. The hash table,
the graph and the tree are all written by the team.


## Build

Compile from the project directory with g++ using C++11:

    g++ -std=c++11 -Wall -Wextra -o campus.exe Utils.cpp HashTable.cpp FacilityDatabase.cpp CampusGraph.cpp DepartmentTree.cpp main.cpp

There is no makefile and no external library; only the standard C++ headers
are used. The build is expected to finish with no errors and no warnings.


## Run

With no arguments the program reads its data from the directory it is started
in:

    ./campus.exe

On Windows the same command is run as campus.exe from the project folder.

The program also accepts one optional argument naming the data directory. That
directory must hold the four data files and is resolved relative to the
working directory, so no absolute paths are required:

    ./campus.exe testdata/collision

If the argument is left out the current directory is used. A file that cannot
be opened is reported and treated as empty, and the program keeps running with
whatever data it did load.

The four instructor data files are buildings.txt, connections.txt,
facilities.txt and departments.txt. They sit in the project directory next to
the source code and are read in that order, because connections need the
building names and facilities are checked against them.


## Menu

    1. View all facilities
    2. Search facility by ID
    3. Search facilities by name
    4. Search facilities by category
    5. View campus graph
    6. Display graph counts and degrees
    7. Find reachable buildings (BFS)
    8. Add department
    9. Search department
    10. Display department hierarchy
    11. Run preorder traversal
    0. Exit

Every prompt reads a whole line, so names containing spaces work. Blank or
text input at the menu is reported and the menu is shown again. Facilities
must be typed exactly, ignoring capitalization only, so "Room 101" matches
and "room" does not. Building and department names are also matched ignoring
capitalization.


## Directory layout

Everything except the collision test data sits together in one directory:

    main.cpp              entry point, menu, dispatch, building loader
    Facility.h            facility record
    FacilityDatabase.h    facility storage and searches
    FacilityDatabase.cpp  loading, listing, searching
    HashTable.h           custom ID hash table
    HashTable.cpp         hash function, insert, find, diagnostic
    CampusGraph.h         campus graph declarations
    CampusGraph.cpp       graph loading, display, counts, BFS
    DepartmentTree.h      department tree declarations
    DepartmentTree.cpp    tree loading, add, search, hierarchy, preorder
    Utils.h               string helper declarations
    Utils.cpp             trimming, case conversion, splitting, ID check
    buildings.txt         instructor data, read at startup
    connections.txt       instructor data, read at startup
    facilities.txt        instructor data, read at startup
    departments.txt       instructor data, read at startup
    design_document.txt   design write-up
    test_results.txt      recorded test runs
    Contributions.txt     team member modules
    testdata/collision/   small data set that produces a hash collision

testdata/collision holds its own buildings.txt and facilities.txt, used to
show the collision diagnostic and the duplicate-ID rejection. It has no
connections.txt or departments.txt, so the program reports those two files as
missing and continues with an empty graph and an empty tree. Run it with the
command shown above. campus.exe is produced by the build command and is not
part of the submission.
