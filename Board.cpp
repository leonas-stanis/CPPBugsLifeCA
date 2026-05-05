#include "Board.h"
#include "Crawler.h"
#include "Hopper.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>


Board::Board() = default;

Board::~Board() {
    for (Bug* bug : bugs) {
        delete bug;
    }
    bugs.clear();
}

void Board::loadFromFile(const string& filename) {
    // Clean up any previously loaded bugs
    for (Bug* bug : bugs) {
        delete bug;
    }
    bugs.clear();

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        vector<string> fields;
        stringstream ss(line);
        string field;
        while (getline(ss, field, ';')) {
            fields.push_back(field);
        }

        if (fields.size() < 6) continue;

        char type = fields[0][0];
        int id = stoi(fields[1]);
        int x = stoi(fields[2]);
        int y = stoi(fields[3]);
        int dir = stoi(fields[4]);
        int health = stoi(fields[5]);

        Bug* bug = nullptr;
        if (type == 'C' || type == 'c') {
            bug = new Crawler(id, {x, y}, dir, health);
        } else if (type == 'H' || type == 'h') {
            int hopLength = (fields.size() >= 7) ? stoi(fields[6]) : 2;
            bug = new Hopper(id, {x, y}, dir, health, hopLength);
        }

        if (bug) {
            bugs.push_back(bug);
        }
    }

    file.close();
    cout << "Loaded " << bugs.size() << " bugs from file '" << filename << "'.\n";
}
void Board::displayAllBugs() const {
    if (bugs.empty()) {
        cout << "No bugs on the board. Load a file first.\n";
        return;
    }

    cout << "\n--- All Bugs ---\n";
    for (const Bug* bug : bugs) {
        // id, type, position, health, direction, (hopLength for hoppers), status
        cout << bug->getId() << " "
             << bug->getType() << " ("
             << bug->getPosition().first << ","
             << bug->getPosition().second << ") "
             << bug->getHealth() << " "
             << bug->getDirectionStr() << " ";

        // Show hopLength for Hopper bugs
        const Hopper* hopper = dynamic_cast<const Hopper*>(bug);
        if (hopper) {
            cout << hopper->getHopLength() << " ";
        }

        cout << (bug->isAlive() ? "Alive" : "Dead") << "\n";
    }
    cout << "----------------\n";
}

void Board::findBug(int id) const {
    for (const Bug* bug : bugs) {
        if (bug->getId() == id) {
            cout << "\nBug " << id << " found:\n";
            cout << "Type: " << bug->getType() << "\n";
            cout << "Position: (" << bug->getPosition().first << ","
                 << bug->getPosition().second << ")\n";
            cout << "Direction: " << bug->getDirectionStr() << "\n";
            cout << "Health: " << bug->getHealth() << "\n";
            cout << "Status: " << (bug->isAlive() ? "Alive" : "Dead") << "\n";

            const Hopper* hopper = dynamic_cast<const Hopper*>(bug);
            if (hopper) {
                cout << "Hop Length: " << hopper->getHopLength() << "\n";
            }
            return;
        }
    }
    cout << "Bug " << id << " not found.\n";
}
map<pair<int, int>, vector<Bug*>> Board::getCellOccupancy() const {
    map<pair<int, int>, vector<Bug*>> cellMap;
    for (Bug* bug : bugs) {
        if (bug->isAlive()) {
            cellMap[bug->getPosition()].push_back(bug);
        }
    }
    return cellMap;
}

void Board::fight() {
    auto cellMap = getCellOccupancy();

    for (auto& [pos, cellBugs] : cellMap) {
        // Only keep alive bugs
        vector<Bug*> aliveInCell;
        for (Bug* b : cellBugs) {
            if (b->isAlive()) aliveInCell.push_back(b);
        }

        if (aliveInCell.size() < 2) continue;

        // Sort by health descending for pairing strategy
        sort(aliveInCell.begin(), aliveInCell.end(), [](Bug* a, Bug* b) {
            return a->getHealth() > b->getHealth();
        });

        cout << "   Cell (" << pos.first << "," << pos.second
             << ") has " << aliveInCell.size() << " bugs fighting!\n";

        // Pair up: (0,1), (2,3), etc.
        for (size_t i = 0; i + 1 < aliveInCell.size(); i += 2) {
            Bug* bug1 = aliveInCell[i];
            Bug* bug2 = aliveInCell[i + 1];

            cout << "   " << bug1->getType() << " " << bug1->getId()
                 << " vs " << bug2->getType() << " " << bug2->getId() << "\n";

            for (int round = 1; round <= 3; round++) {
                if (!bug1->isAlive() || !bug2->isAlive()) break;

                int damage1 = rand() % 6;
                int damage2 = rand() % 6;

                bug1->setHealth(bug1->getHealth() - damage1);
                bug2->setHealth(bug2->getHealth() - damage2);

                if (bug1->getHealth() <= 0) {
                    bug1->setHealth(0);
                    bug1->setAlive(false);
                    cout << "      Bug " << bug1->getId() << " defeated in round " << round << "!\n";
                }
                if (bug2->getHealth() <= 0) {
                    bug2->setHealth(0);
                    bug2->setAlive(false);
                    cout << "      Bug " << bug2->getId() << " defeated in round " << round << "!\n";
                }
            }
        }

        if (aliveInCell.size() % 2 == 1) {
            Bug* lucky = aliveInCell.back();
            cout << "   " << lucky->getType() << " " << lucky->getId()
                 << " has no opponent and remains unscathed.\n";
        }
    }
}

void Board::tap() {
    // Collect indices of alive bugs
    vector<size_t> aliveIndices;
    for (size_t i = 0; i < bugs.size(); i++) {
        if (bugs[i]->isAlive()) {
            aliveIndices.push_back(i);
        }
    }

    if (aliveIndices.empty()) {
        cout << "No alive bugs to move.\n";
        return;
    }

    // Choose one random alive bug to freeze
    size_t frozenIdx = aliveIndices[rand() % aliveIndices.size()];
    cout << "Bug " << bugs[frozenIdx]->getId() << " is frozen and will not move this tap.\n";

    // Move all alive bugs except the frozen one
    for (size_t i = 0; i < bugs.size(); i++) {
        if (bugs[i]->isAlive() && i != frozenIdx) {
            bugs[i]->move();
        }
    }

    cout << "All alive bugs (except frozen) have moved.\n";

    // Fight after movement
    fight();

    // Report alive count and check for end condition
    int alive = countAlive();
    cout << "Alive bugs remaining: " << alive << "\n";
    if (alive <= 1) {
        cout << "\n*** The Last Bug Standing is Bug ";
        for (const Bug* bug : bugs) {
            if (bug->isAlive()) {
                cout << bug->getId() << " (" << bug->getType() << ") ***\n";
                break;
            }
        }
    }
}
void Board::displayLifeHistory() const {
    if (bugs.empty()) {
        cout << "No bugs to display.\n";
        return;
    }

    cout << "\n--- Life History of All Bugs ---\n";
    for (const Bug* bug : bugs) {
        cout << bug->getId() << " " << bug->getType() << " Path: ";

        const auto& path = bug->getPath();
        bool first = true;
        for (const auto& pos : path) {
            if (!first) cout << ",";
            cout << "(" << pos.first << "," << pos.second << ")";
            first = false;
        }

        if (!bug->isAlive()) {
            cout << " Dead";
        } else {
            cout << " Alive!";
        }
        cout << "\n";
    }
    cout << "-------------------------------\n";
}

void Board::displayAllCells() const {
    if (bugs.empty()) {
        cout << "No bugs loaded. Load a file first.\n";
        return;
    }

    cout << "\n--- Cell Occupancy ---\n";
    auto cellMap = getCellOccupancy();

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            cout << "(" << x << "," << y << "): ";
            auto it = cellMap.find({x, y});
            if (it == cellMap.end() || it->second.empty()) {
                cout << "empty\n";
            } else {
                bool first = true;
                for (const Bug* bug : it->second) {
                    if (!first) cout << ", ";
                    cout << bug->getType() << " " << bug->getId();
                    first = false;
                }
                cout << "\n";
            }
        }
    }
    cout << "---------------------\n";
}
void Board::runSimulation() {
    int initialAlive = countAlive();
    if (initialAlive <= 1) {
        cout << "Need at least 2 alive bugs to run simulation.\n";
        return;
    }

    cout << "\n=== Running Simulation (Tap every 1 second) ===\n";
    int tapCount = 0;

    while (countAlive() > 1) {
        tapCount++;
        cout << "\n--- Tap #" << tapCount << " ---\n";
        tap();
        cout << "------------------------\n";
        this_thread::sleep_for(chrono::seconds(1));
    }

    cout << "\n=== Simulation ended after " << tapCount << " taps. ===\n";
    displayLifeHistory();
    writeLifeHistoryToFile();
}

void Board::writeLifeHistoryToFile() const {
    time_t now = time(nullptr);
    char buffer[80];
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, sizeof(buffer), "bugs_life_history_%Y%m%d_%H%M%S.out", timeinfo);

    string filename(buffer);
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not create output file '" << filename << "'\n";
        return;
    }

    file << "Bug Type,ID,Path,Status\n";
    for (const Bug* bug : bugs) {
        file << bug->getType() << "," << bug->getId() << ",[";
        const auto& path = bug->getPath();
        bool first = true;
        for (const auto& pos : path) {
            if (!first) file << ";";
            file << "(" << pos.first << "," << pos.second << ")";
            first = false;
        }
        file << "],";
        file << (bug->isAlive() ? "Alive" : "Dead") << "\n";
    }

    file.close();
    cout << "Life history written to '" << filename << "'\n";
}

int Board::countAlive() const {
    int count = 0;
    for (const Bug* bug : bugs) {
        if (bug->isAlive()) count++;
    }
    return count;
}