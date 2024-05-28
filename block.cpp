#include "structure/block.h"

Block::Block(Vector2 position) {
    this -> position = position;
}

void Block::Draw() {
    DrawRectangle(position.x, position.y, 5, 5, {172, 183, 142, 255});
}

Rectangle Block::getRect() {
    Rectangle rect;
    rect.x = position.x;
    rect.y = position.y;
    rect.width = 5;
    rect.height = 5;
    return rect;
}