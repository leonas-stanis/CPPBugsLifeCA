#ifndef CPPBUGSLIFECA_KNIGHTBUG_H
#define CPPBUGSLIFECA_KNIGHTBUG_H

#include "Bug.h"

class KnightBug : public Bug {
public:
    KnightBug(int id, pair<int, int> position, int direction, int health);
    void move() override;
    string getType() const override;
};

#endif //CPPBUGSLIFECA_KNIGHTBUG_H
