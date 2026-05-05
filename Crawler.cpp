#include "Crawler.h"
#include <cstdlib>

Crawler::Crawler(int id, pair<int, int> position, int direction, int health)
    : Bug(id, position, direction, health) {}

void Crawler::move() {
    if (!alive) return;

    // If the bug's way is blocked (at an edge facing outward),
    // pick a random direction until it can move forward.
    while (isWayBlocked()) {
        direction = (rand() % 4) + 1; // 1=North, 2=East, 3=South, 4=West
    }

    // Move 1 unit in the current direction
    switch (direction) {
        case NORTH: position.second -= 1; break;
        case EAST:  position.first  += 1; break;
        case SOUTH: position.second += 1; break;
        case WEST:  position.first  -= 1; break;
    }

    // Record the new position in the bug's path history
    path.push_back(position);
}

string Crawler::getType() const {
    return "Crawler";
}
