#include "KnightBug.h"
#include "Constants.h"
#include <cstdlib>
#include <vector>

KnightBug::KnightBug(int id, pair<int, int> position, int direction, int health,
                     int boardWidth, int boardHeight)
    : Bug(id, position, direction, health, boardWidth, boardHeight) {}

void KnightBug::move() {
    if (!alive) return;

    // Knight moves: 8 possible L-shaped moves
    // (dx, dy) pairs: 2 in one direction, 1 perpendicular
    const vector<pair<int, int>> knightMoves = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    // Collect all valid moves (within board bounds)
    vector<pair<int, int>> validMoves;
    for (const auto& move : knightMoves) {
        int newX = position.first + move.first;
        int newY = position.second + move.second;
        if (newX >= 0 && newX < boardWidth && newY >= 0 && newY < boardHeight) {
            validMoves.push_back({newX, newY});
        }
    }

    // If there are valid moves, pick one at random
    if (!validMoves.empty()) {
        int choice = rand() % validMoves.size();
        position = validMoves[choice];
    }
    // If no valid moves (shouldn't happen on 10x10 board), stay put

    // Record the new position in the bug's path history
    path.push_back(position);
}

string KnightBug::getType() const {
    return "KnightBug";
}
