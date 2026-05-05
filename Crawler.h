#ifndef CPPBUGSLIFECA_CRAWLER_H
#define CPPBUGSLIFECA_CRAWLER_H

#include "Bug.h"

class Crawler : public Bug {
public:
    Crawler(int id, pair<int, int> position, int direction, int health,
            int boardWidth = 10, int boardHeight = 10);
    void move() override;
    string getType() const override;
};

#endif //CPPBUGSLIFECA_CRAWLER_H
