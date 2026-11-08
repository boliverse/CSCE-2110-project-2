# CSCE-2110-project-2

CSCE 2110 Project 2
Smart Campus Navigation and Facility Analytics System

Author: Bobby Oliver
Updated: November 8, 2026


Purpose

This program loads campus data from plain text files and gives the user a menu
for working with it. At the moment it reads buildings.txt and facilities.txt,
then lets the user list every facility, look one up by ID, or search by
facility name or category. Facilities are kept in a vector for listing and in
a hand-written hash table for ID lookup.

The campus graph and the department tree are not written yet. Options 5
through 11 are reserved for that work and print a placeholder message, so the
menu numbering will not change when those modules are added. connections.txt
and departments.txt are therefore not read yet either.


Build

Compile from the project directory with g++ using C++11:

    g++ -std=c++11 -Wall -Wextra -o campus.exe Utils.cpp HashTable.cpp FacilityDatabase.cpp main.cpp

There is no makefile and no external library; only the standard C++ headers
are used. The build is expected to finish with no errors and no warnings.


Run

With no arguments the program reads its data from the directory it is started
in:

    ./campus.exe

On Windows the same command is run as campus.exe from the project folder.

The program also accepts one optional argument naming the data directory. That
directory must hold buildings.txt and facilities.txt and is resolved relative
to the working directory, so no absolute paths are required:

    ./campus.exe testdata/collision

If the argument is left out the current directory is used. A directory that
does not exist is reported and treated as empty, and the program keeps running
with an empty facility list.

The four instructor data files are buildings.txt, connections.txt,
facilities.txt and departments.txt. They sit in the project directory next to
the source code.


Menu

    1. View all facilities
    2. Search facility by ID
    3. Search facilities by name
    4. Search facilities by category
    5. View campus graph (not built yet)
    6. Display graph counts and degrees (not built yet)
    7. Find reachable buildings (not built yet)
    8. Add department (not built yet)
    9. Search department (not built yet)
    10. Display department hierarchy (not built yet)
    11. Run preorder traversal (not built yet)
    0. Exit

Every prompt reads a whole line, so names containing spaces work. Blank or
text input at the menu is reported and the menu is shown again.


Directory layout

Everything except the collision test data sits together in one directory:

    main.cpp              entry point, building loader, menu, dispatch
    Facility.h            facility record
    FacilityDatabase.h    facility storage and searches
    FacilityDatabase.cpp  loading, listing, searching
    HashTable.h           custom ID hash table
    HashTable.cpp         hash function, insert, find, diagnostic
    Utils.h               string helper declarations
    Utils.cpp             trimming, case conversion, splitting, ID check
    buildings.txt         instructor data, read at startup
    facilities.txt        instructor data, read at startup
    connections.txt       instructor data, not read yet
    departments.txt       instructor data, not read yet
    design_document.txt   design write-up
    test_results.txt      recorded test runs
    Contributions.txt     team member modules
    testdata/collision/   small data set that produces a hash collision

testdata/collision holds its own buildings.txt and facilities.txt, used to
show the collision diagnostic and the duplicate-ID rejection. Run it with the
command shown above. campus.exe is produced by the build command and is not
part of the submission.
