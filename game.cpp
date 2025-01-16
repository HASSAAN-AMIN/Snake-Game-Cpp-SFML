#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <ctime>
#include <string>

using namespace sf;
using namespace std;

const int columns = 45; // total columns in game
const int rows = 30; // total rows in game
const int tile = 24;  // a pixel or a box we can say
const int windowWidth = tile * columns; // 1080
const int windowHeight = tile * rows;  //  720

int initial_size = 15;  // initial length of snake when game starts
int direction; // an integer to show the directions when pressing keys
int snakeLength = initial_size; // not hardcoding because it is being used later for restarting game


int score = 0 ; // initial score
int apple = 5 ; // the value of score increase when eats apple

int snakeX[columns*rows] ; // columns * rows are the maximum number of positions thats why 
int snakeY[columns*rows] ; // making that array

int fruitX, fruitY; // x and y coordinate of the fruit


// Prototypes of the unctions used 
void updateSnake(Music &apple_m);
void drawGrid(RenderWindow &window);
void drawScore(RenderWindow &window , Font &font);
void invinvibleMode(RenderWindow &window  , bool &is_game , bool &isgameover);
bool initializeMusic(Music &backgroundMusic);
/**
 * Main function for the Snake Game.
 * Initializes game settings, loads resources, and manages the game loop.
 * Handles the main menu, options menu, and game states.
 * Allows the player to navigate menus, start and control the snake game, and adjust settings.
 * Displays game graphics, manages player input, and updates the game state.
 * Closes the game window upon user request or when an error occurs.
 */

int main(){
    //seeding the time
    srand(time(0));
    
    //  options things in menu
    const int options_items = 4;
    string options[options_items] = {"Invinvible Mode : ON", "Difficulty : Easy" , "Show Grid : ON" , "Go Back"};
    //  things in menu
    const int MENU_ITEMS = 3; 
    string menuOptions[MENU_ITEMS] = {"PLAY", "Options", "Exit"};
    
    // checks the selection which item is selected in both menu and options
    int currentSelection = 0; 

    //Font for full game
    Font font;
    if (!font.loadFromFile("assets/hy.ttf")) {
        cout << "Failed to load font!" << endl;
        return 0;
    }
    //menu text style
    Text menuText[MENU_ITEMS];
    for (int i = 0; i < MENU_ITEMS; i++) {
        menuText[i].setFont(font);
        menuText[i].setString(menuOptions[i]);
        menuText[i].setCharacterSize(60);
        menuText[i].setPosition(windowWidth / 12, (windowWidth / 7) + i * 150);
        menuText[i].setFillColor(Color(0, 0, 0)); 
    }
    menuText[currentSelection].setFillColor(Color(0,0,100)); 
    
    // Options Text Style
    Text OptionsText[options_items];
    for (int i = 0; i < options_items; i++) {
        OptionsText[i].setFont(font);
        OptionsText[i].setString(options[i]);
        OptionsText[i].setCharacterSize(50);
        OptionsText[i].setPosition(windowWidth / 12, (windowWidth / 7) + i * 150);
        OptionsText[i].setFillColor(Color(0, 0, 0)); 
    }
    OptionsText[currentSelection].setFillColor(Color(0,0,100)); 

    //some Bools i will use 
    bool invinvible = 1 ;
    bool show_grid = 1 ;
    bool  is_menu = true;
    bool  is_game = false;
    bool is_options = false;
    bool isgameover = false;

    //Menu Background setup Below
    Texture menu_back;
    Sprite menu_back_sprite;
    if (!menu_back.loadFromFile("assets/123.jpg")){ 
        cout << "Failed to load background texture!" << endl;
        return 0;
    }
    menu_back_sprite.setTexture(menu_back);
    menu_back_sprite.setScale(1.07f ,0.95f);

    // Music Which runs in Background
    Music backgroundMusic;
    if (!initializeMusic(backgroundMusic))
        exit(0);
    // Music created when apple is eaten
    Music apple_m;
    if (!apple_m.openFromFile("assets/apple.wav")) {
        cout << "Error loading Apple music!" << endl;
    }
    //Rendering the Window
    RenderWindow window(VideoMode(windowWidth, windowHeight), "Snake Game!");
    window.setPosition(Vector2i(500, 200));

    // The Background of game and the snakes body and fruits body everything a square :)
    RectangleShape backgroundRectangle(Vector2f(windowWidth, windowHeight));
    backgroundRectangle.setFillColor(Color(255,255,255));
    RectangleShape snakeRectangle(Vector2f(tile, tile));
    snakeRectangle.setFillColor(Color::Blue);
    RectangleShape fruit_rectangle(Vector2f(tile, tile));
    fruit_rectangle.setFillColor(Color::Green);
    //clock
    Clock gameClock;
    float timer = 0, delay = 0.05f;
    
    // initial x and y coord of fruit
    fruitX = 10;
    fruitY = 10;


    // THE GAME LOOP WHERE GAME WORKS
    while (window.isOpen())
    {   
        window.clear();
        
        
        // Main Menu Loop
        if (is_menu) {
            // Draw the menu background
            window.draw(menu_back_sprite);

            // Draw all the menu items
            for (int i = 0; i < MENU_ITEMS; i++) {
                window.draw(menuText[i]);
            }

            // Handle events
            Event event;
            while (window.pollEvent(event)) {
                // Close the window if the user closes it
                if (event.type == Event::Closed)
                    window.close();

                // Handle key presses
                if (event.type == Event::KeyPressed) {
                    // Move up or down in the menu
                    if (Keyboard::isKeyPressed(Keyboard::Up)) {
                        // Reset the color of the current selection
                        menuText[currentSelection].setFillColor(
                            Color(0, 0, 0)); 

                        // Move up in the menu (wrap around to the end)
                        currentSelection = (currentSelection - 1 + MENU_ITEMS) %MENU_ITEMS;

                        // Highlight the new selection
                        menuText[currentSelection].setFillColor(
                            Color(0,0,100)); 
                    } else if (Keyboard::isKeyPressed(Keyboard::Down)) {
                        // Reset the color of the current selection
                        menuText[currentSelection].setFillColor(
                            Color(0, 0, 0)); 

                        // Move down in the menu (wrap around to the start)
                        currentSelection =
                            (currentSelection + 1) % MENU_ITEMS;

                        // Highlight the new selection
                        menuText[currentSelection].setFillColor(
                            Color(0,0,100)); 
                    } else if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                        // Handle menu selection
                        switch (currentSelection) {
                        case 0:             // Start Game
                            is_menu = false;
                            is_game = true; // Switch to game state
                            currentSelection = 0;
                            break;
                        case 1:
                            is_options = true;
                            is_menu = false;
                            currentSelection = 0;
                            break;
                        case 2:   
                            window.close(); // Close the game
                        }
                    }
                }
            }
        }

        // Options menu state
        if (is_options) {

            // Draw the options menu background
            window.draw(menu_back_sprite);

            // Draw all the options menu items
            for (int i = 0; i < options_items; i++) {
                window.draw(OptionsText[i]);
            }

            // Handle events
            Event event;
            while (window.pollEvent(event)){
                // Close the window if the user closes it
                if (event.type == Event::Closed)
                    window.close();
            
                // Handle key presses
                if (event.type == Event::KeyPressed) {

                    // Up arrow key
                    if (Keyboard::isKeyPressed(Keyboard::Up)) {
                        // Reset the color of the current selection
                        OptionsText[currentSelection].setFillColor(
                            Color(0, 0, 0)); 
                        // Move up in the menu (wrap around to the end)
                        currentSelection = (currentSelection - 1 + options_items) %
                                           options_items;
                        // Highlight the new selection
                        OptionsText[currentSelection].setFillColor(
                            Color(0,0,100)); 
                    }

                    // Down arrow key
                    else if (Keyboard::isKeyPressed(Keyboard::Down)) {
                        // Reset the color of the current selection
                        OptionsText[currentSelection].setFillColor(
                            Color(0, 0, 0)); 
                        // Move down in the menu (wrap around to the start)
                        currentSelection =
                            (currentSelection + 1) % options_items;
                        // Highlight the new selection
                        OptionsText[currentSelection].setFillColor(
                            Color(0,0,100)); 
                    }

                    // Enter key
                    else if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                        // Handle options menu selection
                        switch (currentSelection) {
                        case 0:
                            // Toggle invinvible mode
                            if( options[0] == "Invinvible Mode : ON"){
                                options[0] = "Invinvible Mode : OFF";
                                OptionsText[0].setString(options[0]);
                                invinvible = 0 ;
                            }else if( options[0] == "Invinvible Mode : OFF"){
                                options[0] = "Invinvible Mode : ON";
                                OptionsText[0].setString(options[0]);
                                invinvible = 1 ;
                            }
                            break;
                        case 1:
                            // Cycle through difficulty levels
                            if( options[1] == "Difficulty : Easy"){
                                options[1] = "Difficulty : Hard";
                                OptionsText[1].setString(options[1]);
                                delay = 0.025f;
                            }else if( options[1] == "Difficulty : Hard"){
                                options[1] = "Difficulty : Super Hard";
                                OptionsText[1].setString(options[1]);
                                delay = 0.01f;
                            }else if( options[1] == "Difficulty : Super Hard"){
                                options[1] = "Difficulty : Easy";
                                OptionsText[1].setString(options[1]);
                                delay = 0.05f;
                            }break;
                        case 2:   
                            // Toggle show grid option
                            if( options[2] == "Show Grid : ON"){
                                options[2] = "Show Grid : OFF";
                                OptionsText[2].setString(options[2]);
                                show_grid = 0 ;
                            }else if( options[2] == "Show Grid : OFF"){
                                options[2] = "Show Grid : ON";
                                OptionsText[2].setString(options[2]);
                                show_grid = 1 ;
                            }break;
                        case 3:
                            // Go back to the main menu
                            is_menu = true;
                            is_options = false;
                            currentSelection = 1;
                            break;
                        }
                    }
                }
            }
        }

        if(is_game){
            // Game loop
            // Draw background and grid
            window.draw(backgroundRectangle);
            if(show_grid){
                drawGrid(window); // Draw grid if show_grid is true
            }
            
            // Draw fruit
            fruit_rectangle.setPosition(fruitX * tile, fruitY * tile);
            window.draw(fruit_rectangle);
            
            // Get time elapsed since last frame
            float elapsedTime = gameClock.getElapsedTime().asSeconds();
            gameClock.restart();
            timer += elapsedTime;
            
            // Handle events
            Event event;
            while (window.pollEvent(event)){
                if (event.type == Event::Closed)
                    window.close();
                if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                    is_menu = true;
                    is_game = false;
                    break;
                }
            }

            // Update snake direction based on user input
            if (Keyboard::isKeyPressed(Keyboard::Left)  && direction != 2) {
                direction = 1;
            }
            if (Keyboard::isKeyPressed(Keyboard::Right) && direction != 1) {
                direction = 2;
            }
            if (Keyboard::isKeyPressed(Keyboard::Up)    && direction != 0) {
                direction = 3;
            }
            if (Keyboard::isKeyPressed(Keyboard::Down)  && direction != 3) {
                direction = 0;
            }

            // Update snake if enough time has passed
            if (timer > delay){
                timer = 0;
                updateSnake( apple_m);
            }
            
            // Check if snake has collided with itself or the edge of the screen
            if( !invinvible){
                invinvibleMode(window  , is_game ,isgameover);
            }

            // Draw snake
            for (int i = 0; i < snakeLength; i++){
                float scale = 1.0f;
                if(i == snakeLength - 1) {
                    scale = 0.3f;
                } else if (i == snakeLength - 2) {
                    scale = 0.5f;
                } else if (i == snakeLength - 3) {
                    scale = 0.7f;
                } else if (i == snakeLength - 4){
                    scale =  0.85f;
                } 
                
                // Calculate offset based on scale
                float offset = tile * (1 - scale) / 2;
                snakeRectangle.setScale(scale, scale);
                snakeRectangle.setPosition(snakeX[i] * tile + offset, snakeY[i] * tile + offset);
                
                if(i == 0) { // Head
                    snakeRectangle.setFillColor(Color::Red);
                } else if(i >= snakeLength - 3) { 
                    snakeRectangle.setFillColor(Color(0, 0, 255, 200 + (i - (snakeLength - 3)) * 18));
                } else {
                    snakeRectangle.setFillColor(Color::Blue);
                }
                drawScore(window , font);
                window.draw(snakeRectangle);
            }
        }
        
        // Check if game is over
        if(isgameover){

            // Set background color to black
            window.clear(Color::Black);

            // Draw background rectangle (grid)
            window.draw(backgroundRectangle);

            // Draw grid if option is selected
            if(show_grid){drawGrid(window);}

            // Draw fruit rectangle
            window.draw(fruit_rectangle);
            
            // Draw each part of the snake
            for (int i = 0; i < snakeLength; i++){
                // Set scale based on position in snake
                float scale = 1.0f;
                if(i == snakeLength - 1) {
                    scale = 0.3f;
                } else if (i == snakeLength - 2) {
                    scale = 0.5f;
                } else if (i == snakeLength - 3) {
                    scale = 0.7f;
                } else if (i == snakeLength - 4){
                    scale =  0.85f;
                } 
                
                // Calculate offset based on scale
                float offset = tile * (1 - scale) / 2;
                
                // Set position of snake rectangle based on snakeX and snakeY arrays
                snakeRectangle.setScale(scale, scale);
                snakeRectangle.setPosition(snakeX[i] * tile + offset, snakeY[i] * tile + offset);
                
                // Set color of snake rectangle based on position in snake
                if(i == 0) { // Head
                    snakeRectangle.setFillColor(Color::Red);
                } else if(i >= snakeLength - 3) { 
                    snakeRectangle.setFillColor(Color(0, 0, 255, 200 + (i - (snakeLength - 3)) * 18));
                } else {
                    snakeRectangle.setFillColor(Color::Blue);
                }
                
                // Draw snake rectangle
                window.draw(snakeRectangle);
            }
            
            // Handle user input
            Event event;
            while (window.pollEvent(event)){
                if (event.type == Event::Closed)
                    window.close();
            }
            
            // Draw score
            drawScore(window , font);

            // Create text object for "GAME OVER"
            Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("GAME OVER");
            gameOverText.setCharacterSize(80);
            gameOverText.setPosition((window.getSize().x / 2) - 150, (window.getSize().y / 2) - 50);
            gameOverText.setFillColor(Color(0,0,100));

            // Draw "GAME OVER" text
            window.draw(gameOverText);
            
            // Check for enter key press
            if(Keyboard::isKeyPressed(Keyboard::Enter)){
                // Reset score and snake length
                score = 0;
                snakeLength = initial_size;

                // Reset menu and game state
                currentSelection = 0 ;
                is_menu = true;
                is_game = false;
                isgameover = false;
                invinvible = 0;

                // Reset snake arrays
                for (int i = 0; i < 250; i++) {
                    snakeX[i] = 0; 
                    snakeY[i] = 0;  
                }
            }
        }

        
        
        
        window.display();
    }

    return 0;
}


/**
 * Draws a grid of gray lines on the RenderWindow. The grid is drawn by creating
 * horizontal and vertical lines across the window, with each line being one
 * pixel wide and with a 100% transparent gray color (#80808000).
 * @param window The RenderWindow object to draw the grid onto.
 */
void drawGrid(RenderWindow &window) {
    for (int i = 0; i <= columns; i++) {
        RectangleShape verticalLine(Vector2f(1, windowHeight));
        verticalLine.setPosition(i * tile, 0);
        verticalLine.setFillColor(
            Color(200, 200, 200, 100)); 
        window.draw(verticalLine);
    }

    for (int i = 0; i <= rows; i++) {
        RectangleShape horizontalLine(Vector2f(windowWidth, 1));
        horizontalLine.setPosition(0, i * tile);
        horizontalLine.setFillColor(
            Color(200, 200, 200, 100));
        window.draw(horizontalLine);
    }
}
/**
 * Updates the state of the snake game: moves the snake, checks for collisions with the
 * edge of the screen and the snake's own body, and handles eating the fruit. If the snake
 * eats the fruit, the score increases and the fruit is respwaned at a random location on
 * the screen.
 * @param apple_m The sound effect to be played when the snake eats the fruit.
 */
void updateSnake( Music &apple_m){


        //wrapping around sccreen logic
    if (snakeX[0] >= columns) snakeX[0] = 0;  
    else if (snakeX[0] < 0) snakeX[0] = columns - 1;
    if (snakeY[0] >= rows) snakeY[0] = 0;
    else if (snakeY[0] < 0) snakeY[0] = rows - 1;

    
    bool isFruitOnSnake = false;
    if (snakeX[0] == fruitX && snakeY[0] == fruitY){
        
        do {  // Making sure that the fruit doesn't spawn on the snake's body
            isFruitOnSnake = false;
            fruitX = rand() % columns;
            fruitY = rand() % rows;

            for (int i = 0; i < snakeLength; i++) {
                if (fruitX == snakeX[i] && fruitY == snakeY[i]) {
                    isFruitOnSnake = true;
                    break;
                }
            }
        } while (isFruitOnSnake);
        snakeLength++;
        score += apple;
        apple_m.play();
    }

    //this below for loop works like it starts from last 
    //and from ith index it will assign snakeX[i] = snakeX[i-1] 
    for (int i = snakeLength; i > 0; --i){
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }

            if (direction == 0) {
                snakeY[0]++;
            }else if (direction == 1) {
                snakeX[0]--;
            }else if (direction == 2) {
                snakeX[0]++;
            }else if (direction == 3) {
                snakeY[0]--;
            }

    


}

/**
 * Draws the score on the screen.
 * @param window The RenderWindow object.
 * @param font The font to use for drawing the score.
 */
void drawScore(RenderWindow &window , Font &font) {
    Text scoreText;
    scoreText.setString("Score: " + to_string(score));
    scoreText.setFont(font);
    scoreText.setFillColor(Color::Red);
    scoreText.setCharacterSize(30);
    scoreText.setPosition(10, 10);  
    window.draw(scoreText);
}

/**
 * Checks if the snake has collided with itself, and if so, sets the 
 * corresponding flags to end the game and show the game over screen.
 * @param window The RenderWindow object.
 * @param is_game A boolean indicating whether the game is currently running.
 * @param isgameover A boolean indicating whether the game is over.
 */
void invinvibleMode(RenderWindow &window , bool &is_game , bool &isgameover){
    for (int i = 1; i < snakeLength; i++) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {   
            is_game = false;
            isgameover = true;
        }
    }
}



    /**
    * Initializes the background music.
    * Loads the background music from the specified file and sets its
    * properties (looping, volume, etc.). If the file could not be loaded,
    * an error message is printed to the console and the function returns
    * false.
    *
    * @param backgroundMusic The sf::Music object to be initialized.
    *
    * @return True if the music was loaded successfully, false otherwise.
    */
bool initializeMusic(Music &backgroundMusic) {
    if (!backgroundMusic.openFromFile("assets/Forest.wav")) {
        cout << "Error loading background music!" << endl;
        return false;
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.play();
    backgroundMusic.setVolume(20);
    return true;
}
