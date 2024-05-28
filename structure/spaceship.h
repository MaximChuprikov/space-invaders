#pragma once
#include <raylib.h>
#include "laser.h"
#include <vector>

#ifndef SPACESHIP_H
#define SPACESHIP_H

class Spaceship {
public:
        Spaceship();
        ~Spaceship();
        void Draw();
        void MoveLeft();
        void MoveRight();
        void FireLaser();
        Rectangle getRect();
        void Reset();
        std::vector<Laser> lasers;
private:
        Texture2D image;
        Vector2 position;
        double lastFireTime;
        Sound laserSound;
        Rectangle collisionRect;

};

#endif //SPACESHIP_H