#ifndef __MY_GAME_H__
#define __MY_GAME_H__


#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include "SDL_ttf.h"



static struct GameData {
    int player1Y = 0;
    int player2Y = 0;
    int ballY = 0;
    int ballX = 0;
    int powerUpX;
    int powerUpY;
    bool powerUpActive;
    
} game_data;

class MyGame {

    private:
        SDL_Rect player1{ 800 / 4, 600 / 2 - 30, 20, 100 }; 
        SDL_Rect player2{ 3 * 800 / 4 - 20, 600 / 2 - 30, 20, 100 }; 
        SDL_Rect ball{ 800 / 2 - 30, 600 / 2 - 30, 20, 20 }; 
        SDL_Rect powerUp{ 800 / 2, 600 / 2, 80, 80 }; // 40, 40

        TTF_Font* font;

        int player1Score;
        int player2Score;
        
    public:
        std::vector<std::string> messages;

        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update();
        void render(SDL_Renderer* renderer);
        void setFont(TTF_Font* f) { font = f; }
      
};

//To do: 
//Look at client-side prediction 
// add additional clien/ player
//Add a countdown display for the power up 
//Add a menu, pause screen
//Add sounds
//Add visual effects








#endif
