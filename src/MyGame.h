#include "MyGame.h"





MyGame::~MyGame() {
   

    if (scoreSound != nullptr) {
        Mix_FreeChunk(scoreSound);
    }
    Mix_CloseAudio();

        if (font != nullptr) {
            TTF_CloseFont(font);
        }
        TTF_Quit();


       
    }





void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
    //std::cout << "Received command: " << cmd << std::endl;

    try {
        if (cmd == "GAME_DATA") {
            if (args.size() == 5 || args.size() == 8) {
                game_data.player1Y = std::stoi(args.at(0));
                game_data.player2Y = std::stoi(args.at(1));
                game_data.player3X = std::stoi(args.at(2));
                game_data.ballX = std::stoi(args.at(3));
                game_data.ballY = std::stoi(args.at(4));

                // Check if power-up data is included
                if (args.size() == 8) {
                    game_data.powerUpX = std::stoi(args.at(5));
                    game_data.powerUpY = std::stoi(args.at(6));
                    game_data.powerUpActive = std::stoi(args.at(7)) == 1; // Check if active
                }
                else {
                    game_data.powerUpActive = false;
                }
            }
            else {
                std::cout << "Invalid GAME_DATA received" << std::endl;
            }
        }

        if (cmd == "SCORES" && args.size() == 3) {
            if (scoreSound != nullptr) {
                Mix_PlayChannel(-1, scoreSound, 0);
            }
            player1Score = std::stoi(args.at(0));
            player2Score = std::stoi(args.at(1));
            player3Score = std::stoi(args.at(2));
        }
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        // Ignore this command
        return;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Argument out of range: " << e.what() << std::endl;
        // Ignore this command
        return;
    }
}




void MyGame::init()
{


    //Initialise font
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        return;
        
    }

    // Load the font
    TTF_Font* font = TTF_OpenFont("../res/fonts/pong.ttf", 72);
    if (font == NULL) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        TTF_Quit();
        return;
    }
   
    setFont(font);
  

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        
    }


    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0)
        std::cout << "Failed to init SDL_mixer:" << std::endl;


    // Load score sound effect
    scoreSound = Mix_LoadWAV("../res/sounds/ding.wav");
    if (!scoreSound) {
        std::cerr << "Failed to load score sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

    // Load powerup sound effect
    powerUpSound = Mix_LoadWAV("../res/sounds/time_stop.wav");
    if (!powerUpSound) {
        std::cerr << "Failed to load powerUp sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
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
        case SDLK_f: 
            send(event.type == SDL_KEYDOWN ? "F_DOWN" : "F_UP");
            break;
        case SDLK_g:
            send(event.type == SDL_KEYDOWN ? "G_DOWN" : "G_UP");
            break;
    }
    
}

void MyGame::update() {
    player1.y = game_data.player1Y;
    player2.y = game_data.player2Y;
    player3.x = game_data.player3X;

    ball.x = game_data.ballX;
    ball.y = game_data.ballY;

    if (game_data.powerUpActive) {

        powerUp.x = game_data.powerUpX;
        powerUp.y = game_data.powerUpY;
    }


    // Check for collision with the ball
    if (SDL_HasIntersection(&ball, &powerUp)) {
        game_data.powerUpActive = false;
        if (powerUpSound != nullptr) {
            Mix_PlayChannel(-1, powerUpSound, 0);
        }
    }
}


void MyGame::render(SDL_Renderer* renderer) {

    if (game_data.powerUpActive) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for power-up
        SDL_RenderFillRect(renderer, &powerUp);
    }

    // Player 1 Bat - Red
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
    SDL_RenderFillRect(renderer, &player1);

    // Player 2 Bat - Blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue color
    SDL_RenderFillRect(renderer, &player2);

    // Player 3 Bat - Green
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color
    SDL_RenderFillRect(renderer, &player3);


    // Render the ball as a filled circle
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set the drawing color to white
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
  

    // Player 1 Score
    SDL_Color textColorPlayer1 = { 255, 0, 0, 255 };
    std::string scorePlayer1Text = std::to_string(player1Score);
    SDL_Surface* surfacePlayer1 = TTF_RenderText_Solid(font, scorePlayer1Text.c_str(), textColorPlayer1);
    SDL_Texture* texturePlayer1 = SDL_CreateTextureFromSurface(renderer, surfacePlayer1);
    SDL_Rect textRectPlayer1 = { 50, 50, surfacePlayer1->w, surfacePlayer1->h }; // Position near top-left
    SDL_RenderCopy(renderer, texturePlayer1, NULL, &textRectPlayer1);
    SDL_FreeSurface(surfacePlayer1);
    SDL_DestroyTexture(texturePlayer1);

    // Player 2 Score
    SDL_Color textColorPlayer2 = { 0, 0, 255, 255 };
    std::string scorePlayer2Text = std::to_string(player2Score);
    SDL_Surface* surfacePlayer2 = TTF_RenderText_Solid(font, scorePlayer2Text.c_str(), textColorPlayer2);
    SDL_Texture* texturePlayer2 = SDL_CreateTextureFromSurface(renderer, surfacePlayer2);
    SDL_Rect textRectPlayer2 = { 800 - 50 - surfacePlayer2->w, 50, surfacePlayer2->w, surfacePlayer2->h }; // Position near top-right
    SDL_RenderCopy(renderer, texturePlayer2, NULL, &textRectPlayer2);
    SDL_FreeSurface(surfacePlayer2);
    SDL_DestroyTexture(texturePlayer2);

    // Player 3 Score
    SDL_Color textColorPlayer3 = { 0, 255, 0, 255 };
    std::string scorePlayer3Text = std::to_string(player3Score);
    SDL_Surface* surfacePlayer3 = TTF_RenderText_Solid(font, scorePlayer3Text.c_str(), textColorPlayer3);
    SDL_Texture* texturePlayer3 = SDL_CreateTextureFromSurface(renderer, surfacePlayer3);
    SDL_Rect textRectPlayer3 = { 800 - 375 - surfacePlayer3->w, 50, surfacePlayer3->w, surfacePlayer3->h }; // Position middle
    SDL_RenderCopy(renderer, texturePlayer3, NULL, &textRectPlayer3);
    SDL_FreeSurface(surfacePlayer3);
    SDL_DestroyTexture(texturePlayer3);


}


void MyGame::renderMenu(SDL_Renderer* renderer) {
    // Clear previous bounds
    menuItems.clear();

    std::vector<std::string> texts = { "Resume", "Options", "Exit" };
    for (size_t i = 0; i < texts.size(); ++i) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, texts[i].c_str(), { 255, 255, 255, 255 });
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        int textWidth = surface->w;
        int textHeight = surface->h;
        SDL_Rect textRect = { (800 - textWidth) / 2, 200 + static_cast<int>(i) * 50, textWidth, textHeight };

        // Store the bounds
        menuItems.push_back({ texts[i], textRect });

        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

void MyGame::handleMenuClick(int mouseX, int mouseY)
{

    if (showControls) {
        // Assuming the "Back" button is the last item in controlTexts
        SDL_Rect& backBounds = menuItems.back().bounds;
        if (mouseX >= backBounds.x && mouseX <= backBounds.x + backBounds.w &&
            mouseY >= backBounds.y && mouseY <= backBounds.y + backBounds.h) {
            showControls = false;
            isMenuActive = true;
            return; // Return early
        }
    }
    else
    for (size_t i = 0; i < menuItems.size(); ++i) {
        SDL_Rect& itemBounds = menuItems[i].bounds;
        if (mouseX >= itemBounds.x && mouseX <= itemBounds.x + itemBounds.w &&
            mouseY >= itemBounds.y && mouseY <= itemBounds.y + itemBounds.h) {
            executeMenuItem(i);
            break;
        }


    }
}

void MyGame::executeMenuItem(int itemIndex) {
    switch (itemIndex) {
    case 0: // "Resume" selected
        isMenuActive = false;
        break;
    case 1: // "Options" selected
        showControls = true;
        break;
    case 2: // "Exit" selected
        requestGameExit();
        break;
    }
}

void MyGame::renderControls(SDL_Renderer* renderer) {
    int fontSize = 24; // Smaller font size
    TTF_Font* controlsFont = TTF_OpenFont("../res/fonts/pong.ttf", fontSize);
    if (controlsFont == NULL) {
        std::cerr << "Failed to load smaller font size: " << TTF_GetError() << std::endl;
        return;
    }

    // Control instructions
    std::vector<std::string> controlTexts = {
        "Player 1: W/S to move up/down",
        "Player 2: I/K to move up/down",
        "Player 3: F/G to move left/right",
        "Back" // Add a back button at the end
    };

    int yOffset = 50;
    for (size_t i = 0; i < controlTexts.size(); ++i) {
        SDL_Surface* surface = TTF_RenderText_Solid(controlsFont, controlTexts[i].c_str(), { 255, 255, 255, 255 });
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        int textWidth = surface->w;
        int textHeight = surface->h;

        // Center the text
        SDL_Rect textRect = { (800 - textWidth) / 2, yOffset, textWidth, textHeight };
        yOffset += textHeight + 10; // Increment yOffset for the next text

        SDL_RenderCopy(renderer, texture, NULL, &textRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    TTF_CloseFont(controlsFont); // Close the font after rendering all texts
}


