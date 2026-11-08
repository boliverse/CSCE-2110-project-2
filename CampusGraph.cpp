
#include "CampusGraph.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <queue>

using namespace std;

string CampusGraph::trim(const string& value) {
    size_t first = value.find_first_not_of(" \t\r\n");

    if (first == string::npos) {
        return "";
    }

    size_t last = value.find_last_not_of(" \t\r\n");

    return value.substr(first, last - first + 1);
}

string CampusGraph::lower(const string& value) {
    string result = value;

    for (size_t i = 0; i < result.size(); ++i) {
        result[i] = static_cast<char>(
            tolower(static_cast<unsigned char>(result[i])));
    }

    return result;
}

vector<string> CampusGraph::split(
    const string& line, char delimiter) {

    vector<string> fields;
    size_t start = 0;

    while (true) {
        size_t position = line.find(delimiter, start);

        if (position == string::npos) {
            fields.push_back(trim(line.substr(start)));
            break;
        }

        fields.push_back(
            trim(line.substr(start, position - start)));

        start = position + 1;
    }

    return fields;
}

int CampusGraph::findBuilding(const string& name) const {
    string wanted = lower(trim(name));

    for (size_t i = 0; i < buildings.size(); ++i) {
        if (lower(buildings[i].name) == wanted) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

bool CampusGraph::hasConnection(int first, int second) const {
    const vector<int>& neighbors = buildings[first].neighbors;

    return find(neighbors.begin(), neighbors.end(), second)
           != neighbors.end();
}

void CampusGraph::loadBuildings(const string& filename) {
    ifstream input(filename.c_str());

    int accepted = 0;
    int rejected = 0;
    string line;
    int lineNumber = 0;

    if (!input.is_open()) {
        cout << "Error: could not open " << filename
             << ". Treating it as empty.\n";
        cout << filename << ": 0 accepted, 0 rejected\n";
        return;
    }

    while (getline(input, line)) {
        ++lineNumber;

        string cleaned = trim(line);

        if (cleaned.empty() || cleaned[0] == '#') {
            continue;
        }

        if (cleaned.find('|') != string::npos) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": expected one building name.\n";
            ++rejected;
            continue;
        }

        string name = trim(cleaned);

        if (name.empty()) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": building name is empty.\n";
            ++rejected;
            continue;
        }

        if (findBuilding(name) != -1) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": duplicate building name '"
                 << name << "'.\n";
            ++rejected;
            continue;
        }

        Building building;
        building.name = name;

        buildings.push_back(building);
        ++accepted;
    }

    cout << filename << ": " << accepted
         << " accepted, " << rejected << " rejected\n";
}

void CampusGraph::loadConnections(const string& filename) {
    ifstream input(filename.c_str());

    int accepted = 0;
    int rejected = 0;
    string line;
    int lineNumber = 0;

    if (!input.is_open()) {
        cout << "Error: could not open " << filename
             << ". Treating it as empty.\n";
        cout << filename << ": 0 accepted, 0 rejected\n";
        return;
    }

    while (getline(input, line)) {
        ++lineNumber;

        string cleaned = trim(line);

        if (cleaned.empty() || cleaned[0] == '#') {
            continue;
        }

        vector<string> fields = split(cleaned, '|');

        if (fields.size() != 2) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": expected exactly two fields.\n";
            ++rejected;
            continue;
        }

        if (fields[0].empty() || fields[1].empty()) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": building field is empty.\n";
            ++rejected;
            continue;
        }

        int first = findBuilding(fields[0]);
        int second = findBuilding(fields[1]);

        if (first == -1) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": unknown building '"
                 << fields[0] << "'.\n";
            ++rejected;
            continue;
        }

        if (second == -1) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": unknown building '"
                 << fields[1] << "'.\n";
            ++rejected;
            continue;
        }

        if (first == second) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": self-connections are not allowed.\n";
            ++rejected;
            continue;
        }

        if (hasConnection(first, second)) {
            cout << "Warning: " << filename
                 << " line " << lineNumber
                 << ": duplicate connection.\n";
            ++rejected;
            continue;
        }

        buildings[first].neighbors.push_back(second);
        buildings[second].neighbors.push_back(first);

        ++accepted;
    }

    cout << filename << ": " << accepted
         << " accepted, " << rejected << " rejected\n";
}

void CampusGraph::displayGraph() const {
    if (buildings.empty()) {
        cout << "No buildings are available.\n";
        return;
    }

    cout << "\nCampus Graph\n";

    for (size_t i = 0; i < buildings.size(); ++i) {
        cout << buildings[i].name << ": ";

        if (buildings[i].neighbors.empty()) {
            cout << "(no connections)";
        } else {
            for (size_t j = 0;
                 j < buildings[i].neighbors.size(); ++j) {

                if (j > 0) {
                    cout << ", ";
                }

                cout << buildings[
                    buildings[i].neighbors[j]].name;
            }
        }

        cout << '\n';
    }
}

void CampusGraph::displayCountsAndDegrees() const {
    size_t totalDegree = 0;

    cout << "\nGraph Counts and Degrees\n";

    for (size_t i = 0; i < buildings.size(); ++i) {
        size_t degree = buildings[i].neighbors.size();

        totalDegree += degree;

        cout << buildings[i].name
             << ": degree " << degree << '\n';
    }

    cout << "Total buildings: " << buildings.size() << '\n';
    cout << "Total connections: " << totalDegree / 2 << '\n';
}

void CampusGraph::findReachable(
    const string& startName) const {

    int start = findBuilding(startName);

    if (start == -1) {
        cout << "Error: unknown building '"
             << trim(startName) << "'.\n";
        return;
    }

    vector<bool> visited(buildings.size(), false);
    vector<int> distance(buildings.size(), -1);

    queue<int> pending;

    visited[start] = true;
    distance[start] = 0;
    pending.push(start);

    cout << "\nReachable buildings from "
         << buildings[start].name << ":\n";

    int reachableCount = 0;

    while (!pending.empty()) {
        int current = pending.front();
        pending.pop();

        cout << buildings[current].name
             << " (distance " << distance[current] << ")\n";

        ++reachableCount;

        for (size_t i = 0;
             i < buildings[current].neighbors.size(); ++i) {

            int next = buildings[current].neighbors[i];

            if (!visited[next]) {
                visited[next] = true;
                distance[next] = distance[current] + 1;
                pending.push(next);
            }
        }
    }

    cout << "Reachable: " << reachableCount
         << " of " << buildings.size()
         << " buildings\n";
}

void CampusGraph::findShortestPath(
    const string& startName,
    const string& destinationName) const {

    int start = findBuilding(startName);
    int destination = findBuilding(destinationName);

    if (start == -1) {
        cout << "Error: unknown start building '"
             << trim(startName) << "'.\n";
        return;
    }

    if (destination == -1) {
        cout << "Error: unknown destination building '"
             << trim(destinationName) << "'.\n";
        return;
    }

    vector<bool> visited(buildings.size(), false);
    vector<int> previous(buildings.size(), -1);

    queue<int> pending;

    visited[start] = true;
    pending.push(start);

    while (!pending.empty() && !visited[destination]) {
        int current = pending.front();
        pending.pop();

        for (size_t i = 0;
             i < buildings[current].neighbors.size(); ++i) {

            int next = buildings[current].neighbors[i];

            if (!visited[next]) {
                visited[next] = true;
                previous[next] = current;
                pending.push(next);
            }
        }
    }

    if (!visited[destination]) {
        cout << "No path exists between "
             << buildings[start].name << " and "
             << buildings[destination].name << ".\n";
        return;
    }

    vector<int> path;

    for (int at = destination;
         at != -1;
         at = previous[at]) {

        path.push_back(at);
    }

    reverse(path.begin(), path.end());

    cout << "\nShortest path:\n";

    for (size_t i = 0; i < path.size(); ++i) {
        cout << buildings[path[i]].name << '\n';
    }

    cout << "Number of connections: "
         << path.size() - 1 << '\n';
}

void CampusGraph::connectivityReport() const {
    if (buildings.empty()) {
        cout << "No buildings are available.\n";
        cout << "Total buildings: 0\n";
        cout << "Total connections: 0\n";
        cout << "Highest degree: N/A\n";
        cout << "Isolated buildings: None\n";
        cout << "Connected components: 0\n";
        return;
    }

    size_t totalDegree = 0;
    size_t highestDegree = 0;

    vector<bool> visited(buildings.size(), false);
    vector<int> isolated;
    vector<int> highest;

    for (size_t i = 0; i < buildings.size(); ++i) {
        size_t degree = buildings[i].neighbors.size();

        totalDegree += degree;

        if (degree == 0) {
            isolated.push_back(static_cast<int>(i));
        }

        if (degree > highestDegree) {
            highestDegree = degree;
            highest.clear();
            highest.push_back(static_cast<int>(i));
        } else if (degree == highestDegree) {
            highest.push_back(static_cast<int>(i));
        }
    }

    int components = 0;

    for (size_t i = 0; i < buildings.size(); ++i) {
        if (visited[i]) {
            continue;
        }

        ++components;

        queue<int> pending;

        visited[i] = true;
        pending.push(static_cast<int>(i));

        while (!pending.empty()) {
            int current = pending.front();
            pending.pop();

            for (size_t j = 0;
                 j < buildings[current].neighbors.size(); ++j) {

                int next = buildings[current].neighbors[j];

                if (!visited[next]) {
                    visited[next] = true;
                    pending.push(next);
                }
            }
        }
    }

    cout << "\nGraph Connectivity Report\n";

    cout << "Total buildings: "
         << buildings.size() << '\n';

    cout << "Total connections: "
         << totalDegree / 2 << '\n';

    cout << "Highest degree (" << highestDegree << "): ";

    for (size_t i = 0; i < highest.size(); ++i) {
        if (i > 0) {
            cout << ", ";
        }

        cout << buildings[highest[i]].name;
    }

    cout << "\nIsolated buildings: ";

    if (isolated.empty()) {
        cout << "None";
    } else {
        for (size_t i = 0; i < isolated.size(); ++i) {
            if (i > 0) {
                cout << ", ";
            }

            cout << buildings[isolated[i]].name;
        }
    }

    cout << "\nConnected components: "
         << components << '\n';
}
