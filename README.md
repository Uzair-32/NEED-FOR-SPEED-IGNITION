# Need For Speed: Ignitiion

"Need for Speed: Ignition" is a simple car racing game developed as a first-semester C++ project. This game introduces players to the thrilling experience, where they control a car to avoid obstacles and make a hightscore. The project was designed to showcase fundamental programming skills, game logic, and creativity, all while providing an engaging and fun gaming experience.

## Group Members:
- Muhammad Uzair (Team Lead, System Flow)
- Ayesha Mobeen (Sound Mechanism, HUD)
- Ahmad Mahad Khan Niazi (Time Mechanism, Resource Manager)
- Hamza Raheem (2x Developer, Optimization)

## Featues:
1. Car and Map selection
2. Checkpoints management
3. Pause and Save functionality
4. Fuel and Power-ups system
5. Time tracking 
6. Profile management

## Technolgies Used:
### Programming Languages:
    C++
### Graphics Libraries:
    Simple and Fast Multimedia Libraries
    It is collection of different C++ libraries that are used to create a simple 2-Dimensional games. 
    
## Demo: 
[![Demo video](https://img.youtube.com/vi/video-id/0.jpg)](https://www.youtube.com/watch?v=video-id)

## How it Works: 
1. The game initializes with a menu section where user can either start, select a car or change the settings.
2. After selecting a car, the user chooses a map. 
3. The player navigates a car using keyboard inputs (e.g., arrow keys).
4. Obstacles (i.e. Cars) appear randomly on the screen, moving toward the player.
5. The player earns points for successfully avoiding obstacles.
6. The player has 10 lives, and for each collision a live is deducted. After all the 10 lives are used, the game ends. The game also ends when the user runs out of the fuel. 
7. After the game has ended, the game displays the player's score, time survived, cause of lose, and a menu to restart or continue from the last checkpoint. 

## Challenges Faced: 
1. Designing collision detection logic for the car and obstacles.
2. Implementing dynamic speed adjustments to increase difficulty.
3. Ensuring smooth and responsive controls.

## Learning Outcomes: 
1. Gained practical experience in object-oriented programming (OOP).
2. Applied concepts like loops, conditionals, vectors and arrays.
3. Enhanced problem-solving and debugging skills.
4. Learned basic game mechanics and UI design.

## Documentation: 
The following section contains the documentation about the purpose and functionality of the variables and functions used in the code.

### Constants and Global Variables
- fontPath: Path to the font file.
- collisionSoundPath: Path to the collision sound file.
- backgroundMusicPath: Path to the background music file.
- carShapePath: Path to the car shape image.
- backgroundTrack: Path to the background track image.
- menuTitleImage: Path to the menu title image.
- pauseTitleImage: Path to the pause title image.
- carImage1 to carImage6: Paths to the car images.
- clickSound: Path to the click sound file.
- save_load: Path to the settings file.
- countDownSoundPath: Path to the countdown sound file.
- trackWidth: Width of the track.
- trackHeight: Height of the track.
- lastCheckpointScore: Last checkpoint score.
- passedCheckpoint: Flag to check if the player passed a checkpoint.
- selectedCarTexture: Texture for the selected car.

### Functions

- #### void checkForCheckpoint(int& score)
    ```
    Checks if the player has passed a checkpoint based on the score and updates the lastCheckpointScore and passedCheckpoint variables.
    ```
- #### void countDown(RenderWindow& window, Font& font)
    ```
    Displays a countdown before the game starts. It plays a countdown sound and updates the countdown text on the screen.
    ```
- #### void resetToCheckpoint(int& lives, int& score, float& currentFuel, vector<RectangleShape>& obstacles, Sprite& carSprite, const int trackWidth, const int trackHeight, bool& carSelected, vector<CircleShape>& powerUps)
    ```
    Resets the game state to the last checkpoint. It updates the score, lives, fuel, car position, and repositions obstacles and power-ups.
    ```
    
- #### void drawRoundedButton(RenderWindow& window, Vector2f position, Vector2f size, const string& text, Font& font, bool isHovered, Color fillColor, Color hoverColor)
    ```
    Draws a rounded button with the specified text, position, size, and colors. It also handles hover effects.
    ```
- #### void carSelection(RenderWindow& window, Font font, bool& carSelected, Texture& selectedCarTexture)
    ```
    Displays the car selection menu where the player can choose a car. It handles car selection and updates the selectedCarTexture.
    ```

- #### void menuSection(RenderWindow& window, Font& font, Texture& selectedCarTexture, bool ispaused, bool& carSelected, Music& backgroundMusic)
    ```
    Displays the main menu with options to start the game, select a car, change settings, and quit the game.
    ```

- #### void generateObstacles(vector<RectangleShape>& obstacles, int numObstacles, int obstacleSize, Vector2f trackBounds)
    ```
    Generates obstacles at random positions within the track bounds.
    ```

- #### void addProfile(RenderWindow& window, Font& font, Sound& clickSound)
    ```
    Displays a UI to add a new player profile. It handles user input for the profile name and saves it to a file.
    ```

- #### void savePlayTime(float playTime)
    ```
    Saves the playtime to a file.
    ```

- #### float loadPlayTime()
    ```
    Loads the playtime from a file.
    ```
- #### void managePowerUps(RenderWindow& window, Sprite& carSprite, vector<CircleShape>& powerUps, float& currentFuel, int& score, int& speedBoostDuration, int& lives, int powerUpSize, Vector2f trackBounds)
    ```
    Manages power-ups by generating them based on score milestones, drawing them, and handling collisions with the car.
    ```

- #### void resetGame(int& lives, int& score, float& currentFuel, vector<RectangleShape>& obstacles, Sprite& carSprite, const int trackWidth, const int trackHeight, bool& carSelected, vector<CircleShape>& powerUps, float& totalTime)
    ```
    Resets the game state, including lives, score, fuel, car position, obstacles, power-ups, and total time.
    ```

- #### void tryAgain(RenderWindow& window, Font& font, bool& isPaused, bool& carSelected, int& lives, int& score, float& currentFuel, vector<RectangleShape>& obstacles, Sprite& carSprite, const int trackWidth, const int trackHeight, Music& backgroundMusic, vector<CircleShape>& powerUps, float totalTime)
    ```
    Displays the "Game Over" screen with options to try again, return to the main menu, or exit the game. It also shows the final score and total time.
    ```

- #### void showChooseCarMessage(RenderWindow& window, Font& font, bool& carSelected)
    ```
    Displays a message prompting the player to select a car before starting the game.
    ```
- #### void settingsWindow(RenderWindow& window, Font& font, Texture& selectedCarTexture, bool& carSelected, bool ispaused, Music& backgroundMusic)
    ```
    Displays the settings window where the player can adjust music settings, volume, fullscreen mode, and add profiles.
    ```
- #### int showPauseMenu(RenderWindow& window, Font& font, bool& ispaused, Texture selectedCarTexture, bool carSelected, Music& backgroundMusic)
    ```
    Displays the pause menu with options to resume the game, change settings, return to the main menu, or exit the game.
    ```

### main() function:

The main function initializes the game window, loads assets, and starts the game loop. It handles player input, updates the game state, and renders the game.