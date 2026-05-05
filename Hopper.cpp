#include "Hopper.h"
#include <cstdlib>

Hopper::Hopper(int id, pair<int, int> position, int direction, int health, int hopLength)
    : Bug(id, position, direction, health), hopLength(hopLength) {}

void Hopper::move() {
    if (!alive) return;

    // If way is blocked, pick a random direction until it can move forward
    while (isWayBlocked()) {
        direction = (rand() % 4) + 1;
    }

    // Try to move hopLength units in current direction
    int newX = position.first;
    int newY = position.second;

    switch (direction) {
        case NORTH:
            newY -= hopLength;
            if (newY < 0) newY = 0;
            break;
        case EAST:
            newX += hopLength;
            if (newX > 9) newX = 9;
            break;
        case SOUTH:
            newY += hopLength;
            if (newY > 9) newY = 9;
            break;
        case WEST:
            newX -= hopLength;
            if (newX < 0) newX = 0;
            break;
    }

    position.first = newX;
    position.second = newY;

    // Record the new position in the bug's path history
    path.push_back(position);
}

string Hopper::getType() const {
    return "Hopper";
}

int Hopper::getHopLength() const {
    return hopLength;
}
