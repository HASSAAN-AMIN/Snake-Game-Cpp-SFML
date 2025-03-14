# Snake-Game-Cpp-SFML
A simple Snake game created in C++ using the SFML library.

## Gameplay Link
https://www.linkedin.com/posts/hassaan4133_cpp-sfml-gamedevelopment-activity-7285765806842384385-dMDR?utm_source=social_share_send&utm_medium=android_app&rcm=ACoAACyns1MBR-ecKYIS35k7n1dJuxTqBK5R1H8&utm_campaign=copy_link

## Game Description

This is a classic Snake Game where the player controls a snake moving around a grid-based game board. The goal is to eat fruits (apples) that randomly appear on the board, increasing the snake's length and the player's score. The game includes a main menu, an options menu, and various game states for enhanced player interaction.

### Features:
- Grid-based movement.
- Randomly spawning fruits.
- Score tracking.
- Main menu and options menu.
- Game over and restart functionality.
- Background music integration.

## Code Overview

### Highlights:
1. **Game Settings and Constants**:  
   Configurable constants for board size, tile dimensions, and snake behavior.
   
2. **Game Loop and Main Function**:  
   The main function initializes the game, handles input, updates game states, and renders graphics.

3. **Key Functions**:  
   - `updateSnake`: Updates the snake's position, handles collisions, and checks for fruit consumption.  
   - `drawGrid`: Renders a grid on the game board.  
   - `drawScore`: Displays the player's current score.  
   - `invincibleMode`: Handles collision detection and game-over logic.  
   - `initializeMusic`: Loads and plays background music.

4. **SFML Integration**:  
   Utilizes SFML for handling graphics, input, and audio. Features include `RenderWindow`, `Texture`, `Sprite`, and `Font` for rendering assets and UI.

## How to Compile and Run the Game

Follow the instructions below to get the game up and running.

### Prerequisites
Make sure you have:
- A C++ compiler (e.g., `g++` or `clang`).
- The SFML library installed on your system. Refer to the official [SFML Tutorial](https://www.sfml-dev.org/tutorials/3.0/) for installation steps.

## For Linux
### Install the GNU G++ Compiler
```sudo apt-get install g++```

### Install SFML
```sudo apt-get install libsfml-dev```

### Compile the game
```g++ -o Game game.cpp -lsfml-graphics -lsfml-window -lsfml-system```

### Run the game
```./Game```

