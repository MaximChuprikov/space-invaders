#include "structure/spaceship.h"

Color white = {255, 255, 255, 255};
Spaceship::Spaceship()
{
    image = LoadTexture("/Users/max/CLionProjects/space invaders/images/spaceship.png");
    position.x = (GetScreenWidth() - image.width)/2;
    position.y = GetScreenHeight() - image.height - 100;
    lastFireTime = 0.0;
    laserSound = LoadSound("/Users/max/CLionProjects/untitled1/Sounds/laser.ogg");
    collisionRect.x = position.x + 20;  // Отступ слева 20 пикселей
    collisionRect.y = position.y + 20;  // Отступ сверху 20 пикселей
    collisionRect.width = image.width - 50;  // Ширина на 40 пикселей меньше
    collisionRect.height = image.height - 100;  // Высота на 40 пикселей меньше
}

Spaceship::~Spaceship() {
    UnloadTexture(image);
    UnloadSound(laserSound);
}

void Spaceship::Draw() {
    DrawTextureV(image, position, white);
}

void Spaceship::MoveLeft() {
    position.x -= 7;
    if(position.x < 25) {
        position.x = 25;
    }
}

void Spaceship::MoveRight() {
    position.x += 7;
    if(position.x > GetScreenWidth() - image.width) {
        position.x = GetScreenWidth() - image.width - 25;
    }
}

void Spaceship::FireLaser()
{
    if (GetTime()- lastFireTime >= 0.35) {
        lasers.push_back(Laser({position.x + image.width/2 - 2, position.y},-6));
        lastFireTime = GetTime();
        PlaySound(laserSound);

    }
}

Rectangle Spaceship::getRect() {
    return {position.x, position.y, float(image.width - 50), float(image.height - 50)};
}
void Spaceship::Reset() {
    position.x = (GetScreenWidth() - image.width) / 2.0f;
    position.y = GetScreenHeight() - image.height - 100;
    lasers.clear();
}