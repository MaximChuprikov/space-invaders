#include "structure/alien.h"


Alien::Alien(int type, Vector2 position): health(2), isShaking(false), shakeTimer(0.0f), shakeIntensity(0.0f) {
    this -> type = type;
    this -> position = position;
    switch (type) {
        case 1:
            image = LoadTexture("/Users/max/CLionProjects/space invaders/images/asteroid1.png");
        break;
        case 2:
            image = LoadTexture("/Users/max/CLionProjects/space invaders/images/asteroid2.png");
        break;
        case 3:
            image = LoadTexture("/Users/max/CLionProjects/space invaders/images/asteroid3.png");
        break;
        case 4:
            image = LoadTexture("/Users/max/CLionProjects/space invaders/images/alien.png");
        break;
        default:
            image = LoadTexture("/Users/max/CLionProjects/space invaders/images/alien.png");
        break;
    }
}

void Alien::Draw() {
    Vector2 drawPos = position;
    if (isShaking) {
        drawPos.x += GetRandomValue(-shakeIntensity, shakeIntensity);
        drawPos.y += GetRandomValue(-shakeIntensity, shakeIntensity);
    }
    DrawTextureV(image, drawPos, WHITE);
}
int Alien::GetType() {
    return type;
}

Rectangle Alien::getRect() {
    return {position.x, position.y,
    float(image.width - 10),
        float(image.height)
    };
}
void Alien::Update(float direction) {
    position.x += direction;
}

void Alien::TakeDamage() {
    health--;
}

// Метод для получения текущего здоровья
int Alien::GetHealth() const {
    return health;
}
void Alien::StartShake(float duration, float intensity) {
    isShaking = true;
    shakeTimer = duration;
    shakeIntensity = intensity;
}
void Alien::UpdateShake(float delta) {
    if (isShaking) {
        shakeTimer -= delta;
        if (shakeTimer <= 0.0f) {
            isShaking = false;
            shakeTimer = 0.0f;
        }
    }
}