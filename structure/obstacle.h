#pragma once
#include <vector>
#include "block.h"

#ifndef OBSTACLE_H
#define OBSTACLE_H

class Obstacle {
public:
    Obstacle(Vector2 position);
    void Draw();
    Vector2 position;
    std::vector<Block> blocks;
    static std::vector<std::vector<int>> grid;
private:

};

#endif //OBSTACLE_H