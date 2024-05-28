#include "structure/game.h"
#include <iostream>
#include <fstream>

Game::Game() {
    InitGame();
}


Game::~Game() {
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}

void Game::Update() {
    if (run) {
        for(auto& laser: spaceship.lasers) {
            laser.Update();
        }
        MoveAliens();

        AlienShootLaser();

        for (auto& laser: alienLasers) {
            laser.Update();
        }

        DeleteInactiveLasers();

        CheckForCollisions();
        // Обновление состояния тряски для всех инопланетян
        for (auto& alien : aliens) {
            alien.UpdateShake(GetFrameTime());
        }
        for (auto& alien : asteroids) {
            alien.UpdateShake(GetFrameTime());
        }
        if (aliens.empty() && asteroids.empty()) {
            isPlayerWon = true;
            run = false;
            StopMusicStream(music);
        }
    } else {
        if(IsKeyDown(KEY_ENTER)) {
            Reset();
            InitGame();
        }
    }
}



void Game::Draw() {
    spaceship.Draw();

    for(auto& laser: spaceship.lasers) {
        laser.Draw();
    }

    for(auto& obstacle: obstacles) {
        obstacle.Draw();
    }
    for(auto& alien: aliens) {
        alien.Draw();
    }
    for(auto& alien: asteroids) {
        alien.Draw();
    }
    for (auto& laser: alienLasers) {
        laser.Draw();
    }
}

void Game::HandleInput() {
    if (run) {
        if(IsKeyDown(KEY_LEFT)) {
            spaceship.MoveLeft();
        } else if (IsKeyDown(KEY_RIGHT)) {
            spaceship.MoveRight();
        } else if (IsKeyDown(KEY_SPACE)) {
            spaceship.FireLaser();
        } else if (IsKeyDown(KEY_R)) { // Добавлено условие для сброса highscore
            ResetHighscore();
        }
    }
}
void Game::DeleteInactiveLasers() {
    for(auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();) {
        if(!it -> active) {
            it = spaceship.lasers.erase(it);
        } else {
            ++ it;
        }
    }
    for(auto it = alienLasers.begin(); it != alienLasers.end();) {
        if(!it -> active) {
            it = alienLasers.erase(it);
        } else {
            ++ it;
        }
    }
}

std::vector<Obstacle> Game::CreateObstacles() {
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (3 * obstacleWidth))/4;
    for(int i = 0; i < 3; i++) {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() - 230)}));

    }
    return obstacles;
}

std::pair<std::vector<Alien>, std::vector<Alien>> Game::CreateAliens() {
    std::vector<Alien> aliens;
    std::vector<Alien> asteroids;
    int alienType;
    for(int row = 0; row < 6; row++) {
        for(int column = 0; column < 13; column++) {
            if ((row + column) % 2 == 0 && row == 0) {
                alienType = 4;
                float x = column * 50 + 99;
                float y = row * 50 + 100;
                aliens.push_back(Alien(alienType, {x, y}));
            } else if ((row + column) % 2 == 0 && row != 0) {
                if (row == 1) {
                    alienType = 3;
                } else if (row == 2 || row == 3) {
                    alienType = 2;
                } else {
                    alienType = 1;
                }
                float x = column * 50 + 99;
                float y = row * 50 + 110;
                asteroids.push_back(Alien(alienType, {x, y}));
            }
        }
    }
    return std::make_pair(aliens, asteroids);
}

void Game::MoveAliens() {
    for(auto& alien: aliens) {
        if(alien.position.x + alien.image.width > GetScreenWidth() - 25) {
            aliensDirection = -0.4;
        }
        if (alien.position.x <= 25) {
            aliensDirection = 0.4;
        }
        MoveDownAliens(0.005);
        alien.Update(aliensDirection);
    }
    for(auto& alien: asteroids) {
        if(alien.position.x + alien.image.width > GetScreenWidth() - 25) {
            aliensDirection = -0.4;
        }
        if (alien.position.x <= 0) {
            aliensDirection = 0.4;
        }
        MoveDownAliens(0.005);
        alien.Update(aliensDirection);
    }
}

void Game::MoveDownAliens(float distance) {
    for(auto& alien: aliens) {
            alien.position.y += distance;
        }
    for(auto& alien: asteroids) {
        alien.position.y += distance;
    }
    }

void Game::AlienShootLaser() {
    double currentTime = GetTime();
    if(currentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty()) {
        int randomIndex = GetRandomValue(0, static_cast<int>(aliens.size()) - 1);
        Alien& alien = aliens[randomIndex];
        alienLasers.push_back(Laser({alien.position.x + alien.image.width/2, alien.position.y + alien.image.height/2}, 6));
        timeLastAlienFired = GetTime();
    }
}

void Game::CheckForCollisions() {
    //Spaceship Lasers
    for (auto& laser : spaceship.lasers) {
        auto it = aliens.begin();
        while (it != aliens.end()) {
            if (CheckCollisionRecs(it->getRect(), laser.getRect())) {
                PlaySound(explosionSound);
                if (it->GetHealth() == 2) { // Если здоровье полное (2), начинаем тряску
                    it->StartShake(0.5f, 10.0f); // Продолжительность тряски 0.5 секунды, интенсивность 10 пикселей
                }
                it->TakeDamage();
                laser.active = false;
                if (it->GetHealth() == 0) {
                    score += 500;
                    checkForHighscore();
                    it = aliens.erase(it);
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }
        for(auto& obstacle: obstacles) {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end()) {
                if (CheckCollisionRecs(it -> getRect(), laser.getRect())) {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                } else {
                    ++it;
                }
            }
        }
        for(auto& laser: spaceship.lasers) {
            auto it = asteroids.begin();
            while(it != asteroids.end()) {
                if(CheckCollisionRecs(it -> getRect(), laser.getRect())) {
                    PlaySound(explosionSound);
                    if (it -> type == 1) {
                        score += 100;
                    } else if (it -> type == 2) {
                        score += 300;
                    } else if (it -> type == 3) {
                        score += 400;
                    }
                    checkForHighscore();
                    it = asteroids.erase(it);
                    laser.active = false;
                } else {
                    ++it;
                }
            }
        }
    }
    // Alien lasers

    for(auto& laser: alienLasers) {
        if(CheckCollisionRecs(laser.getRect(), spaceship.getRect())) {
            laser.active = false;
            lives--;
            if(lives == 0) {
               GameOver();
            }
        }

        for(auto& obstacle: obstacles) {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end()) {
                if (CheckCollisionRecs(it -> getRect(), laser.getRect())) {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                } else {
                    ++it;
                }
            }
        }
    }
    //Alien Collision with Obstacle

    for(auto& alien: aliens) {
        for(auto& obstacle: obstacles) {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()) {
                if(CheckCollisionRecs(it-> getRect(), alien.getRect())) {
                    it = obstacle.blocks.erase(it);
                } else {
                    it++;
                }
            }
        }

        if(CheckCollisionRecs(alien.getRect(), spaceship.getRect())) {
            lives--;
            if(lives == 0) {
                GameOver();
            }
        }
    }
    for(auto& alien: asteroids) {
        for(auto& obstacle: obstacles) {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()) {
                if(CheckCollisionRecs(it-> getRect(), alien.getRect())) {
                    it = obstacle.blocks.erase(it);
                } else {
                    it++;
                }
            }
        }
        if(CheckCollisionRecs(alien.getRect(), spaceship.getRect())) {
            lives--;
            if(lives == 0) {
                GameOver();
            }
        }
    }
}

void Game::GameOver() {
    run = false;
    StopMusicStream(music);
}

void Game::InitGame() {
    obstacles = CreateObstacles();
    aliens = CreateAliens().first;
    asteroids = CreateAliens().second;
    aliensDirection = 0.4;
    timeLastAlienFired = 0.0;
    lives = 3;
    score = 0;
    highscore = loadHighscoreFromFile();
    run = true;
    isPlayerWon = false;
    music = LoadMusicStream("/Users/max/CLionProjects/untitled1/Sounds/music.ogg");
    explosionSound = LoadSound("/Users/max/CLionProjects/untitled1/Sounds/explosion.ogg");
    PlayMusicStream(music);
}

void Game::checkForHighscore() {
    if(score > highscore) {
        highscore = score;
        saveHighscoreToFile(highscore);
    }
}

void Game::saveHighscoreToFile(int highscore) {
    std::ofstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()) {
        highscoreFile << highscore;
        highscoreFile.close();
    } else {
        std::cerr << "Failed to save highscore to file" << std::endl;
    }
}

int Game::loadHighscoreFromFile() {
    int loadedHighscore = 0;
    std::ifstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()) {
        highscoreFile >> loadedHighscore;
        highscoreFile.close();
    } else {
        std::cerr << "Failed to load highscore from file" << std::endl;
    }
    return loadedHighscore;
}

void Game::ResetHighscore() {
    highscore = 0;
    saveHighscoreToFile(0);
}


void Game::Reset() {
    spaceship.Reset();
    aliens.clear();
    asteroids.clear();
    alienLasers.clear();
    obstacles.clear();
    isPlayerWon = false;
}