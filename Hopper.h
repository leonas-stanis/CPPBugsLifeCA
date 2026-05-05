#ifndef CPPBUGSLIFECA_HOPPER_H
#define CPPBUGSLIFECA_HOPPER_H

#include "Bug.h"

class Hopper : public Bug {
private:
    int hopLength;

public:
    Hopper(int id, pair<int, int> position, int direction, int health,
           int hopLength, int boardWidth = 10, int boardHeight = 10);
    void move() override;
    string getType() const override;
    int getHopLength() const;
};

#endif //CPPBUGSLIFECA_HOPPER_H
