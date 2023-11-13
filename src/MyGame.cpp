#include "MyGame.h"




void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
    std::cout << "Received command: " << cmd << std::endl;

    if (cmd == "GAME_DATA") {
        if (args.size() == 4 || args.size() == 7) {
            game_data.player1Y = stoi(args.at(0));
            game_data.player2Y = stoi(args.at(1));
            game_data.ballX = stoi(args.at(2));
            game_data.ballY = stoi(args.at(3));

            // Check if power-up data is included
            if (args.size() == 7) {
                game_data.powerUpX = stoi(args.at(4));
                game_data.powerUpY = stoi(args.at(5));
                game_data.powerUpActive = stoi(args.at(6)) == 1; // Check if active
            }
            else {
                game_data.powerUpActive = false;
            }
        }
        else {
            std::cout << "Invalid GAME_DATA received" << std::endl;
        }
    }

    if (cmd == "SCORES" && args.size() == 2) {
        player1Score = stoi(args.at(0));
        player2Score = stoi(args.at(1));
    }

}



void MyGame::send(std::string message) {
    messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_w:
            send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
            break;
        case SDLK_s:
            send(event.type == SDL_KEYDOWN ? "S_DOWN" : "S_UP");
            break;
        case SDLK_i:
            send(event.type == SDL_KEYDOWN ? "I_DOWN" : "I_UP");
            break;
        case SDLK_k:
            send(event.type == SDL_KEYDOWN ? "K_DOWN" : "K_UP");
            break;
    }
    
}

void MyGame::update() {
    player1.y = game_data.player1Y;
    player2.y = game_data.player2Y;
    ball.x = game_data.ballX;
    ball.y = game_data.ballY;
    
    if (game_data.powerUpActive) {
        powerUp.x = game_data.powerUpX;
        powerUp.y = game_data.powerUpY;
    }


}

void MyGame::render(SDL_Renderer* renderer) {

    if (game_data.powerUpActive) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for power-up
        SDL_RenderFillRect(renderer, &powerUp);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set the drawing color to white
   
    SDL_RenderFillRect(renderer, &player1); 
    SDL_RenderFillRect(renderer, &player2); 



    // Render the ball as a filled circle
    int ballRadius = 20; // Set the radius of the ball
    int ballXCenter = game_data.ballX + ballRadius; // Calculate the center of the ball
    int ballYCenter = game_data.ballY + ballRadius;

    // Draw a filled circle for the ball
    for (int x = -ballRadius; x <= ballRadius; x++) {
        for (int y = -ballRadius; y <= ballRadius; y++) {
            if (x * x + y * y <= ballRadius * ballRadius) {
                SDL_RenderDrawPoint(renderer, ballXCenter + x, ballYCenter + y);
            }
        }
    }

 

    // Render scores
    SDL_Color textColor = { 255, 255, 255 }; // White color

    // Player 1 Score
    std::string scorePlayer1Text = std::to_string(player1Score);
    SDL_Surface* surfacePlayer1 = TTF_RenderText_Solid(font, scorePlayer1Text.c_str(), textColor);
    SDL_Texture* texturePlayer1 = SDL_CreateTextureFromSurface(renderer, surfacePlayer1);
    SDL_Rect textRectPlayer1 = { 50, 50, surfacePlayer1->w, surfacePlayer1->h }; // Position near top-left
    SDL_RenderCopy(renderer, texturePlayer1, NULL, &textRectPlayer1);
    SDL_FreeSurface(surfacePlayer1);
    SDL_DestroyTexture(texturePlayer1);

    // Player 2 Score
    std::string scorePlayer2Text = std::to_string(player2Score);
    SDL_Surface* surfacePlayer2 = TTF_RenderText_Solid(font, scorePlayer2Text.c_str(), textColor);
    SDL_Texture* texturePlayer2 = SDL_CreateTextureFromSurface(renderer, surfacePlayer2);
    SDL_Rect textRectPlayer2 = { 800 - 50 - surfacePlayer2->w, 50, surfacePlayer2->w, surfacePlayer2->h }; // Position near top-right
    SDL_RenderCopy(renderer, texturePlayer2, NULL, &textRectPlayer2);
    SDL_FreeSurface(surfacePlayer2);
    SDL_DestroyTexture(texturePlayer2);




}


