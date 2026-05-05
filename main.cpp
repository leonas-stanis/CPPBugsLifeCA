#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Board.h"

using namespace std;

void displayMenu() {
    cout << "\n===== A Bug's Life - Bug Board Simulator =====\n";
    cout << "1. Initialize Bug Board (load data from file)\n";
    cout << "2. Display all Bugs\n";
    cout << "3. Find a Bug (given an id)\n";
    cout << "4. Tap the Bug Board (move all, then fight/eat)\n";
    cout << "5. Display Life History of all Bugs\n";
    cout << "6. Display all Cells listing their Bugs\n";
    cout << "7. Run simulation (generates a Tap every second)\n";
    cout << "8. Exit (write Life History of all Bugs to file)\n";
    cout << "==============================================\n";
    cout << "Enter your choice: ";
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    Board board;
    int choice;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                board.loadFromFile("bugs.txt");
                break;

            case 2:
                board.displayAllBugs();
                break;

            case 3: {
                int id;
                cout << "Enter bug ID: ";
                cin >> id;
                board.findBug(id);
                break;
            }

            case 4:
                board.tap();
                break;

            case 5:
                board.displayLifeHistory();
                break;

            case 6:
                board.displayAllCells();
                break;

            case 7:
                board.runSimulation();
                break;

            case 8:
                cout << "Exiting...\n";
                board.writeLifeHistoryToFile();
                break;

            default:
                cout << "Invalid choice. Please enter 1-8.\n";
                break;
        }
    } while (choice != 8);

    return 0;
}
