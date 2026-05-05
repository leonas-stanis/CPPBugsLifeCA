//
// Created by user on 05/05/2026.
//

#ifndef CPPBUGSLIFECA_BUG_H
#define CPPBUGSLIFECA_BUG_H

#include <utility>
#include <list>
#include <string>

using namespace std;

enum Direction { NORTH = 1, EAST = 2, SOUTH = 3, WEST = 4 };

class Bug {
protected:
    int id;
    pair<int, int> position;
    int direction;
    int health;
    bool alive;
    int eatenBy;   // ID of the bug that ate this one (-1 if alive / not eaten)
    list<pair<int, int>> path;

public:
    Bug(int id, pair<int, int> position, int direction, int health);
    int getEatenBy() const;
    void setEatenBy(int bugId);
    virtual ~Bug() = default;
    virtual void move() = 0;
    virtual string getType() const = 0;
    bool isWayBlocked();

    int getId() const;
    pair<int, int> getPosition() const;
    int getDirection() const;
    string getDirectionStr() const;
    int getHealth() const;
    bool isAlive() const;
    list<pair<int, int>> getPath() const;

    void setPosition(pair<int, int> newPos);
    void setDirection(int newDir);
    void setHealth(int newHealth);
    void setAlive(bool status);
};

#endif //CPPBUGSLIFECA_BUG_H
