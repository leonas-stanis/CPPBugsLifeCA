#ifndef CPPBUGSLIFECA_BOARD_H
#define CPPBUGSLIFECA_BOARD_H

#include <vector>
#include <string>
#include <map>
#include "Bug.h"

using namespace std;

class Board {
private:
    vector<Bug*> bugs;

    // Build occupancy map of cell -> alive bugs (internal helper)
    map<pair<int, int>, vector<Bug*>> getCellOccupancy() const;

    // Execute fight rounds on bugs sharing the same cell
    void fight();

public:
    Board();
    ~Board();

    // Load bugs from a semicolon-delimited text file
    void loadFromFile(const string& filename);

    // Display all bugs with their details
    void displayAllBugs() const;

    // Find a bug by ID and display its details
    void findBug(int id) const;

    // Tap the board: move all alive bugs (one frozen), then fight
    void tap();

    // Display life history (path) of all bugs
    void displayLifeHistory() const;

    // Display every cell and the bugs occupying it
    void displayAllCells() const;

    // Run simulation: tap every 1 second until one bug remains
    void runSimulation();

    // Write life history to a timestamped file
    void writeLifeHistoryToFile() const;

    // Count how many bugs are still alive
    int countAlive() const;
};

#endif //CPPBUGSLIFECA_BOARD_H
