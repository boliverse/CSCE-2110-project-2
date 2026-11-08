
#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H

#include <string>
#include <vector>

class CampusGraph {
private:
    struct Building {
        std::string name;
        std::vector<int> neighbors;
    };

    std::vector<Building> buildings;

    static std::string trim(const std::string& value);
    static std::string lower(const std::string& value);
    static std::vector<std::string> split(
        const std::string& line, char delimiter);

    int findBuilding(const std::string& name) const;
    bool hasConnection(int first, int second) const;

public:
    void loadBuildings(const std::string& filename);
    void loadConnections(const std::string& filename);

    void displayGraph() const;
    void displayCountsAndDegrees() const;
    void findReachable(const std::string& startName) const;

    void findShortestPath(
        const std::string& startName,
        const std::string& destinationName) const;

    void connectivityReport() const;
};

#endif
