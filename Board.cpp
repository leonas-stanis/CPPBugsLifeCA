#include "Board.h"
#include "Crawler.h"
#include "Hopper.h"

#include <iostream>
#include <fstream>
#include <sstream>


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
