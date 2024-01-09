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

    //// Check for version information command
    //if (cmd == "VERSION_CHECK") {
    //    std::string clientVersion = args.at(0);
    //    if (clientVersion != GAME_VERSION) {
    //        // Handle version mismatch
    //        // Display onscreen message
    //    }
    //}

    //std::cout << "Received command: " << cmd << std::endl;

      //if (cmd == "PLAYER_ID") {
    //  playerId = args.at(0); // Store the received player ID  
    //}

    try {
        if (cmd == "GAME_DATA") {
            if (args.size() == 5 || args.size() == 8) {
                // Update game state based on received data
                game_data.player1Y = std::stoi(args.at(0));
                game_data.player2Y = std::stoi(args.at(1));
                game_data.player3X = std::stoi(args.at(2));
                game_data.ballX = std::stoi(args.at(3));
                game_data.ballY = std::stoi(args.at(4));

                // Check if power-up data is included
                if (args.size() == 8) {
                    game_data.powerUpX = std::stoi(args.at(5));
                    game_data.powerUpY = std::stoi(args.at(6));
                    game_data.powerUpActive = std::stoi(args.at(7)) == 1;
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
            // Play score sound and update scores
            if (scoreSound != nullptr) {
                Mix_PlayChannel(-1, scoreSound, 0);
            }
            player1Score = std::stoi(args.at(0));
            player2Score = std::stoi(args.at(1));
            player3Score = std::stoi(args.at(2));

            // Check if any player has won
            if (player1Score >= 10 || player2Score >= 10 || player3Score >= 10) {
                gameState = WIN_SCREEN;
            }
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





void MyGame::init(SDL_Renderer* renderer)
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


    IMG_Init(IMG_INIT_PNG); 

    // Load the power-up image as a surface
    SDL_Surface* tempSurface = IMG_Load("../res/image/apple.png");
    if (tempSurface == nullptr) {
        std::cerr << "Failed to load power-up image: " << IMG_GetError() << std::endl;
    }
    else {
        // Convert the surface to a texture
        powerUpTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
    }

    // Load the bat1
    SDL_Surface* tempSurfaceBat1 = IMG_Load("../res/image/red.png");
    if (tempSurfaceBat1 == nullptr) {
        std::cerr << "Failed to load bat1 image: " << IMG_GetError() << std::endl;
    }
    else {
         bat1Texture = SDL_CreateTextureFromSurface(renderer, tempSurfaceBat1);
        SDL_FreeSurface(tempSurfaceBat1);
    }

    // Load the bat2
    SDL_Surface*  tempSurfaceBat2 = IMG_Load("../res/image/blue.png");
    if (tempSurfaceBat2 == nullptr) {
        std::cerr << "Failed to load bat2 image: " << IMG_GetError() << std::endl;
    }
    else {
         bat2Texture = SDL_CreateTextureFromSurface(renderer, tempSurfaceBat2);
        SDL_FreeSurface(tempSurfaceBat2);
    }

    // Load the bat3
    SDL_Surface* tempSurfaceBat3 = IMG_Load("../res/image/green.png");
    if (tempSurfaceBat3 == nullptr) {
        std::cerr << "Failed to load bat3 image: " << IMG_GetError() << std::endl;
    }
    else {
       bat3Texture = SDL_CreateTextureFromSurface(renderer, tempSurfaceBat3);
        SDL_FreeSurface(tempSurfaceBat3);
    }
   
}




void MyGame::send(std::string message) {
    messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_p:  // 'P' key for pausing
            showControls = false;
            isPaused = !isPaused;
            break;
        }
    }

    if (isPaused) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                if (currentOption > RESUME) currentOption = (PauseMenuOptions)(currentOption - 1);
                break;
            case SDLK_DOWN:
                if (currentOption < EXIT) currentOption = (PauseMenuOptions)(currentOption + 1);
                break;
            case SDLK_RETURN:
                if (currentOption == RESUME) isPaused = false;
                else if (currentOption == CONTROLS) showControls = true;
                else if (currentOption == EXIT) is_running = false;
                break;

            }

        }
    }

    else {

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
}

void MyGame::createParticle(int x, int y) {
    Particle p;
    p.position = { x, y };
    p.velocity = { rand() % 3 - 1, rand() % 3 - 1 }; // Random velocity
    p.color = { 255, 255, 255, 255 }; // White color
    p.lifespan = 20; 
    particles.push_back(p);
}

void MyGame::updateParticles() {
    for (auto& p : particles) {
        p.position.x += p.velocity.x;
        p.position.y += p.velocity.y;
        p.lifespan--;
    }
    // Remove dead particles
    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [](const Particle& p) { return p.lifespan <= 0; }),
        particles.end());
}


    

void MyGame::update() {
    player1.y = game_data.player1Y;
    player2.y = game_data.player2Y;
    player3.x = game_data.player3X;

    ball.x = game_data.ballX;
    ball.y = game_data.ballY;

    createParticle(ball.x + ball.w / 2, ball.y + ball.h / 2);
    updateParticles();

    if (game_data.powerUpActive) {

        powerUp.x = game_data.powerUpX;
        powerUp.y = game_data.powerUpY;
    }


    if (SDL_HasIntersection(&ball, &powerUp)) {
        game_data.powerUpActive = false;
        playPowerUpSound();  // Play sound on collision
    }
}

void MyGame::playPowerUpSound() {
    if (!Mix_Playing(-1)) {  // Check if no channel is currently playing
        Mix_PlayChannel(-1, powerUpSound, 0);
    }

  
}


void MyGame::renderPlayerScore(SDL_Renderer* renderer, int score, SDL_Color color, int x, int y) {
    std::string scoreText = std::to_string(score);
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


void MyGame::renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void MyGame::renderParticles(SDL_Renderer* renderer) {
    for (const auto& p : particles) {
        SDL_SetRenderDrawColor(renderer, p.color.r, p.color.g, p.color.b, p.color.a);
     

        // Define a small rectangle around the particle's position
        SDL_Rect particleRect = { p.position.x - 2, p.position.y - 2, 4, 4 }; // 4x4 pixel particle

        SDL_RenderFillRect(renderer, &particleRect); // Fill the rectangle (solid particle)
        
    }
    }


void MyGame::render(SDL_Renderer* renderer) {

    if (gameState == PLAYING) {
        if (showControls) {
            int startY = 100; // Starting Y position for the first line of text
            int lineHeight = 50; // Space between lines

            renderText(renderer, "UP1: W, DOWN1: S", 100, startY, { 255, 255, 255, 255 });
            renderText(renderer, "UP2: I, DOWN2: K", 100, startY + lineHeight, { 255, 255, 255, 255 });
            renderText(renderer, "RIGHT3: F, RIGHT3: G", 100, startY + lineHeight *2, { 255, 255, 255, 255 });



        }
        else if (isPaused) {
            const char* menuOptions[] = { "Resume", "Controls", "Exit" };
            int menuY = 200; // Starting Y position for menu items

            for (int i = 0; i < 3; i++) {
                SDL_Color color = (i == currentOption) ? SDL_Color{ 255, 255, 0, 255 } : SDL_Color{ 255, 255, 255, 255 };
                renderText(renderer, menuOptions[i], 350, menuY, color);
                menuY += 50; // Move down for the next option
            }
        

        }

        else {

            if (game_data.powerUpActive) {
                SDL_Rect powerUpRect = { game_data.powerUpX, game_data.powerUpY, 40, 40 }; // Set size of power-up
                SDL_RenderCopy(renderer, powerUpTexture, NULL, &powerUpRect);
            
            }

            // Player 1 Bat - Red
      
            SDL_RenderCopy(renderer, bat1Texture, NULL, &player1); 

            // Player 2 Bat - Blue
            SDL_RenderCopy(renderer, bat2Texture, NULL, &player2);

            // Player 3 Bat - Green
            SDL_RenderCopy(renderer, bat3Texture, NULL, &player3);


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

            // Render particle effect
            renderParticles(renderer); 

      // Render scores

      // Player 1 Score
            renderPlayerScore(renderer, player1Score, { 255, 0, 0, 255 }, 50, 50);

            // Player 2 Score
            renderPlayerScore(renderer, player2Score, { 0, 0, 255, 255 }, 800 - 50, 50); 

            // Player 3 Score
            renderPlayerScore(renderer, player3Score, { 0, 255, 0, 255 }, 800 - 375, 50); 


        }
    }
    

    else if (gameState == WIN_SCREEN) {
        std::string winText = "";
        if (player1Score >= 10) {
            winText = "Player 1 Wins!";
        }
        else if (player2Score >= 10) {
            winText = "Player 2 Wins!";
        }
        else if (player3Score >= 10) {
            winText = "Player 3 Wins!";
        }

        // Calculate text position to center it on the screen
        int textWidth, textHeight;
        TTF_SizeText(font, winText.c_str(), &textWidth, &textHeight);
        int xCenter = (800 - textWidth) / 2;
        int yCenter = (600 - textHeight) / 2;

        renderText(renderer, winText, xCenter, yCenter, { 255, 255, 255, 255 });
    }



 }



