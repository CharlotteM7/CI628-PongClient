#ifndef __MY_GAME_H__
#define __MY_GAME_H__


#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_image.h"



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




struct Particle {
    SDL_Point position;
    SDL_Point velocity;
    SDL_Color color;
    int lifespan;
};




class MyGame {

private:
    SDL_Rect player1{ 800 / 4, 600 / 2 - 30, 20, 100 };
    SDL_Rect player2{ 3 * 800 / 4 - 20, 600 / 2 - 30, 20, 100 };
    SDL_Rect player3{ 800 / 2 - 60 / 2,  600 - 50 - 30, 100, 20 }; // Position at bottom, horizontal bat
    SDL_Rect ball{ 800 / 2 - 30, 0, 20 };
    SDL_Rect powerUp{ 800 / 2, 600 / 2, 40, 40 };

    TTF_Font* font;
    Mix_Chunk* scoreSound;
    Mix_Chunk* powerUpSound;

    SDL_Renderer* renderer;
    SDL_Texture* powerUpTexture;
    SDL_Texture* bat1Texture;
    SDL_Texture* bat2Texture;
    SDL_Texture* bat3Texture;

 
    int player1Score;
    int player2Score;
    int player3Score;


 
    enum PauseMenuOptions { RESUME, CONTROLS, EXIT };
    PauseMenuOptions currentOption = RESUME;

    enum GameState { PLAYING, WIN_SCREEN };
    GameState gameState = PLAYING;


    bool isPaused;
    bool showControls;


public:

    std::vector<std::string> messages;
    std::vector<Particle> particles;
    bool is_running = true;


    ~MyGame();
    void on_receive(std::string message, std::vector<std::string>& args);
    void send(std::string message);
    void input(SDL_Event& event);
    void update();
    void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color);
    void renderPlayerScore(SDL_Renderer* renderer, int score, SDL_Color color, int x, int y);
    void render(SDL_Renderer* renderer);
    void setFont(TTF_Font* f) { font = f; }
    void init(SDL_Renderer* renderer);
    void playPowerUpSound();
    void createParticle(int x, int y);
    void updateParticles();
    void renderParticles(SDL_Renderer* renderer);
};





#endif
