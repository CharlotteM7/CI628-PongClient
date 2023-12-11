#ifndef __MY_GAME_H__
#define __MY_GAME_H__


#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"


static struct GameData {
    int player1Y = 0;
    int player2Y = 0;
    int player3X = 0;
    int ballY = 0;
    int ballX = 0;
    int powerUpX;
    int powerUpY;
    bool powerUpActive;

} game_data;


struct MouseData {

    int mouseX;
    int mouseY;

};





class MyGame {

private:
    SDL_Rect player1{ 800 / 4, 600 / 2 - 30, 20, 100 };
    SDL_Rect player2{ 3 * 800 / 4 - 20, 600 / 2 - 30, 20, 100 };
    SDL_Rect player3{ 800 / 2 - 60 / 2,  600 - 50 - 30, 100, 20 }; // Position at bottom, horizontal bat

    SDL_Rect ball{ 800 / 2 - 30, 600 / 2 - 30, 20, 20 };
    SDL_Rect powerUp{ 800 / 2, 600 / 2, 40, 40 };

    TTF_Font* font;
    Mix_Chunk* scoreSound;
    Mix_Chunk* powerUpSound;

    int player1Score;
    int player2Score;
    int player3Score;

   


public:

    std::vector<std::string> messages;

    bool showControls = false;
    bool isMenuActive = false;
    bool requestExit = false;

    struct MenuItem {
        std::string text;
        SDL_Rect bounds;
    };

    std::vector<MenuItem> menuItems;

    ~MyGame();
    void on_receive(std::string message, std::vector<std::string>& args);
    void send(std::string message);
    void input(SDL_Event& event);
    void update();
    void render(SDL_Renderer* renderer);
    void setFont(TTF_Font* f) { font = f; }
    void init();
    void renderMenu(SDL_Renderer* renderer);
    void handleMenuClick(int mouseX, int mouseY);
    void executeMenuItem(int itemIndex);
    void requestGameExit() { requestExit = true; }
    bool shouldExit() const { return requestExit; }
    void renderControls(SDL_Renderer* renderer);
   
};





#endif
