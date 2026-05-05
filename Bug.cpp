//
// Created by user on 05/05/2026.
//

#include "Bug.h"
#include "Constants.h"

Bug::Bug(int id, pair<int, int> position, int direction, int health,
        int boardWidth, int boardHeight)
    : id(id), position(position), direction(direction), health(health),
      alive(true), eatenBy(-1), boardWidth(boardWidth), boardHeight(boardHeight) {
    path.push_back(position);
}

bool Bug::isWayBlocked() {
    switch (direction) {
        case NORTH:
            return position.second == 0;
        case EAST:
            return position.first == boardWidth - 1;
        case SOUTH:
            return position.second == boardHeight - 1;
        case WEST:
            return position.first == 0;
    }
    return false;
}

int Bug::getId() const { return id; }
pair<int, int> Bug::getPosition() const { return position; }
int Bug::getDirection() const { return direction; }
string Bug::getDirectionStr() const {
    switch (direction) {
        case NORTH: return "North";
        case EAST:  return "East";
        case SOUTH: return "South";
        case WEST:  return "West";
        default:    return "Unknown";
    }
}
int Bug::getHealth() const { return health; }
bool Bug::isAlive() const { return alive; }
list<pair<int, int>> Bug::getPath() const { return path; }

int Bug::getEatenBy() const { return eatenBy; }
void Bug::setEatenBy(int bugId) { eatenBy = bugId; }

void Bug::setPosition(pair<int, int> newPos) { position = newPos; }
void Bug::setDirection(int newDir) { direction = newDir; }
void Bug::setHealth(int newHealth) { health = newHealth; }
void Bug::setAlive(bool status) { alive = status; }

int Bug::getBoardWidth() const { return boardWidth; }
int Bug::getBoardHeight() const { return boardHeight; }
void Bug::setBoardSize(int w, int h) {
    boardWidth = w;
    boardHeight = h;
}
