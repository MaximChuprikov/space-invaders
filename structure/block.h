#pragma once
#include <raylib.h>
#ifndef BLOCK_H
#define BLOCK_H

class Block {
public:
    Block(Vector2 position);
    void Draw();
    Rectangle getRect();
private:
    Vector2 position;

};

#endif //BLOCK_H