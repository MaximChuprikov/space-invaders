#pragma once
#include "spaceship.h"
#include "obstacle.h"
#include "alien.h"
#ifndef GAME_H
#define GAME_H

class Game {
public:
    Game();
    ~Game();
    void Draw();
    void Update();
    void HandleInput();
    bool run;
    int lives;
    int score;
    int highscore;
    Music music;
    bool isPlayerWon;
private:
    void DeleteInactiveLasers();
    std::vector<Obstacle> CreateObstacles();
    std::pair<std::vector<Alien>, std::vector<Alien>> CreateAliens();
    void MoveAliens();
    void MoveDownAliens(float distance);
    void AlienShootLaser();
    void CheckForCollisions();
    void GameOver();

    Spaceship spaceship;
    std::vector<Obstacle> obstacles;
    std::vector<Alien> aliens;
    std::vector<Alien> asteroids;
    float aliensDirection;
    std::vector<Laser> alienLasers;
    constexpr static float alienLaserShootInterval = 0.7;
    float timeLastAlienFired;
    void checkForHighscore();
    void saveHighscoreToFile(int highscore);
    int loadHighscoreFromFile();
    void Reset();
    void InitGame();
    Sound explosionSound;
    void ResetHighscore();

};

#endif //GAME_H