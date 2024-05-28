#pragma once
#include <raylib.h>

#ifndef ALIEN_H
#define ALIEN_H

class Alien {
public:
    Alien(int type, Vector2 position);
    void Update(float direction);
    void Draw();
    int GetType();
    static void UnloadImages();
    Rectangle getRect();
    int type;
    Texture2D image;
    int GetHealth() const;
    void TakeDamage();
    Vector2 position;
    void StartShake(float duration, float intensity); // Новый метод для начала тряски
    void UpdateShake(float delta); // Новый метод для обновления состояния тряски
private:
    int health;
    bool isShaking; // Флаг для состояния тряски
    float shakeTimer; // Таймер для контроля продолжительности тряски
    float shakeIntensity; // Интенсивность тряски (смещение координат)
};

#endif //ALIEN_H