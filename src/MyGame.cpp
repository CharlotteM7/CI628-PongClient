#include "MyGame.h"




void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
    if (cmd == "GAME_DATA") {
        // we should have exactly 4 arguments
        if (args.size() == 4) {
            game_data.player1Y = stoi(args.at(0));
            game_data.player2Y = stoi(args.at(1));
            game_data.ballX = stoi(args.at(2));
            game_data.ballY = stoi(args.at(3));
        }
    } else {
        std::cout << "Received: " << cmd << std::endl;
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
   
  
    
}

void MyGame::render(SDL_Renderer* renderer) {
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
}


