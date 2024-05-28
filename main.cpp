#include "raylib.h"
#include "structure/game.h"
#include <string>


std::string FormatWithLeadingZeros(int number, int width) {
    std::string numberText = std::to_string(number);
    int leadingZeros = width - numberText.length();
    return numberText = std::string(leadingZeros, '0') + numberText;
}
int main()
{
    Color grey = {29, 29, 27, 255};
    Color green = {102, 255, 0, 255};
    Color gold = {255, 215, 0, 255};
    Color red = {255, 0,0, 255};
    int offset = 50;
    int windowWidth = 750;
    int windowHeight = 700;

    InitWindow(windowWidth + offset, windowHeight + 2 * offset, "Space Invaders");
    InitAudioDevice();

    Font font = LoadFontEx("/Users/max/CLionProjects/untitled1/font/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("/Users/max/CLionProjects/untitled1/images/live.png");

    SetTargetFPS(60);

    Game game;

    while(WindowShouldClose() == false) {
        UpdateMusicStream(game.music);
        game.HandleInput();
        game.Update();

        BeginDrawing();
        ClearBackground(grey);
        if (game.run && game.isPlayerWon != 1) {
            DrawTextEx(font, "GAME!", {570, 740}, 34, 2, green);
            DrawRectangleRoundedLines({10, 10, 780, 780}, 0.18f, 20, 2, green);
            DrawLineEx({25,730}, {775, 730}, 3, green);
        } else if (game.isPlayerWon) { // Проверка на победу
            DrawTextEx(font, "YOU WIN!", {275, 350}, 65, 2, gold);
            DrawRectangleRoundedLines({10, 10, 780, 780}, 0.18f, 20, 2, gold);
            DrawLineEx({25, 730}, {775, 730}, 3, gold);
        } else {
            DrawTextEx(font, "GAME OVER", {570, 740}, 34, 2, red);
            DrawRectangleRoundedLines({10, 10, 780, 780}, 0.18f, 20, 2, red);
            DrawLineEx({25,730}, {775, 730}, 3, red);
        }
        float x = 50.0;
        for(int i = 0; i < game.lives; i ++) {
            DrawTextureV(spaceshipImage, {x, 745}, WHITE);
            x += 50;
        }

        DrawTextEx(font, "SCORE", {50, 15}, 34, 2, green);
        std::string scoreText = FormatWithLeadingZeros(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), {35, 40}, 34, 2, green);

        DrawTextEx(font, "HIGH SCORE", {570, 15}, 34, 2, gold);
        std::string highscoreText = FormatWithLeadingZeros(game.highscore, 5);
        DrawTextEx(font, highscoreText.c_str(), {690, 40}, 34, 2, gold);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    CloseAudioDevice();
}