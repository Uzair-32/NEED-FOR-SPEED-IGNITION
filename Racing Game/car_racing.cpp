#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include<iostream>
#include<cmath>
#include<fstream>
#include<iomanip>
#include<sstream>
using namespace std;
using namespace sf;

// Assets Paths
const string fontPath = "assets/fonts/russo-one.ttf";
const string collisionSoundPath = "assets/sounds/collision.wav";
const string backgroundMusicPath = "assets/sounds/background.ogg";
const string carShapePath = "assets/car.png";
const string backgroundTrack = "assets/background/background.png";
const string menuTitleImage = "assets/background/image.png";
const string pauseTitleImage = "assets/background/image1.jpg";
const string carImage1 = "assets/cars/image1.png";
const string carImage2 = "assets/cars/image2.png";
const string carImage3 = "assets/cars/image3.png";
const string carImage4 = "assets/cars/image4.png";
const string carImage5 = "assets/cars/image5.png";
const string carImage6 = "assets/cars/image6.png";
const string clickSound = "assets/sounds/click.wav";
const string save_load = "config/setting.txt";
const string countDownSoundPath = "assets/sounds/countdown.wav";
const string fuelPowerUpPath = "assets/power ups/fuel.png";
const string scorePowerUpPath = "assets/power ups/score.png";
const string livesPowerUpPath = "assets/power ups/lives.png";
const string bgMapTrack1 = "assets/maps/Desert_road.png";
const string bgMapTrack2 = "assets/maps/Highway_road.png";
const string bgMapTrack3 = "assets/maps/Winter_road.png";
const string bgMapTrack4 = "assets/maps/Summer_road.png";
const string obstacle1Path = "assets/obstacles/obstacle1.png";
const string obstacle2Path = "assets/obstacles/obstacle2.jpg";
const string obstacle3Path = "assets/obstacles/obstacle3.jpg";
// constants for track dimensions
const int trackWidth = 1920;
const int trackHeight = 1080;


int lastCheckpointScore = 0;  // last checkpoint score which is initialize as 0 because game start from score=0
bool passedCheckpoint = false;  // variable to check if player passed a checkpoint
Texture selectedCarTexture;   
Texture selectedMapTexture;


// called in start of the main function to show the video first
void introVideo(RenderWindow& window) { 
    const string frames = "assets/intro/ezgif-frame-";// beacuse of many numbers of images in the file i write this as bcz every image start with this name first the only difference is of the numbers at the end
    const int startFrame = 1;  // the first frame name is 1 and last is 67 there are basically 98 images in intro folder
    const int endFrame = 67;
    const int fps = 25;    // video will be played at 25fps

    vector<Texture> framesTexture;
    Sprite framesSprite;
    

    // loop to take the path of the every image from 1 to 98 from intro folder 
    for (int i = startFrame; i <= endFrame; ++i) {
        ostringstream frameFile;
        frameFile << frames << setfill('0') << setw(3) << i << ".png";

        Texture texture;
        if (texture.loadFromFile(frameFile.str())) {
            framesTexture.push_back(move(texture));
        }
        else {
            window.close();
        }
    }


    // if no more frames are left then it will return the fun and the next menu section will be visible.....
    if (framesTexture.empty()) {
        return;
    }

    Clock frameClock; // clock class used to manage the time as per sec how many frames should apear on the screen
    float frameTime = 1.0f / fps;
    size_t currentFrame = 0; // current frame start from 0

    while (window.isOpen() && currentFrame < framesTexture.size()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        // it show the intro video as long as frames remains 

        if (frameClock.getElapsedTime().asSeconds() > frameTime) {
            framesSprite.setTexture(framesTexture[currentFrame]); // use to show the frames in sequence
            framesSprite.setScale(
                static_cast<float>(window.getSize().x) / framesTexture[currentFrame].getSize().x,
                static_cast<float>(window.getSize().y) / framesTexture[currentFrame].getSize().y
            );
            // every time the next frame appear 
            currentFrame++;
            frameClock.restart();
        }

        window.clear();
        window.draw(framesSprite);
        window.display();
    }
}




// this function is used to store the different states of the game like score and lives in a file of txt   (called in setting function)
void saveGame(int& score, int& lives/* string& name*/) {
    ofstream saveFile("config/save.txt");

    if (saveFile.is_open()) {
        /*saveFile << name << endl;*/
        saveFile << score << endl;
        saveFile << lives << endl;
        

        saveFile.close();
    }
    else {
        cout << "There is some problem in opening the file.";
    }
}

// this function is used to load the states of game (called in setting function)
void loadGame(int& score, int& lives) {
    ifstream loadFile("config/save.txt");

    if (loadFile.is_open()) {
        //loadFile >> name;
        loadFile >> score;
        loadFile >> lives;
        

        loadFile.close();
    }
    else {
        cout << "There is some problem in opening the file.";
    }
}






// used to make a new window where user can select a map.
void mapSelection(RenderWindow& window, Font font, Texture& selectedMapTexture) {
    Vector2f buttonSize(400, 80);// i have created a function of draw rounded button , so in every function i first create the different button their name color and hover effect and just pass them to the function
    float cornerRadius = 20.0f;
    Color buttonFillColor = Color(70, 130, 180);   // when there is no hover effect 
    Color hoverColor = Color(100, 149, 237);    // on hover

    // to load the sound of button click
    SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile(clickSound)) {
        cout << "Sound Not Found!";
        window.close();
    }
    Sound clickSound(clickBuffer);

    // background texture for map selection screen
    Texture menuTitlebackgroundTexture;
    if (!menuTitlebackgroundTexture.loadFromFile(menuTitleImage)) {
        cout << "Image not found!" << endl;
        window.close();
    }

    Sprite menuTitlebackgroundTrackSprite;
    menuTitlebackgroundTrackSprite.setTexture(menuTitlebackgroundTexture);
    menuTitlebackgroundTrackSprite.setScale(
        float(1980) / menuTitlebackgroundTexture.getSize().x,
        float(1080) / menuTitlebackgroundTexture.getSize().y
    );

    // helper function to draw rounded buttons
    auto drawRoundedButton = [&](Vector2f position, const string& text, Text& textObject, bool isHovered) {
        Color fillColor = isHovered ? hoverColor : buttonFillColor;

        // Base rectangle
        RectangleShape baseRect(Vector2f(buttonSize.x - 2 * cornerRadius, buttonSize.y));
        baseRect.setFillColor(fillColor);
        baseRect.setPosition(position.x + cornerRadius, position.y);

        // Circles for corners
        CircleShape corner(cornerRadius);
        corner.setFillColor(fillColor);

        // Text
        textObject.setString(text);
        textObject.setCharacterSize(50);
        textObject.setFillColor(Color::White);
        FloatRect textBounds = textObject.getLocalBounds();
        textObject.setPosition(
            position.x + (buttonSize.x - textBounds.width) / 2,
            position.y + (buttonSize.y - textBounds.height) / 2 - 10
        );

        // Draw components
        window.draw(baseRect);

        // Draw corners
        for (const auto& pos : vector<Vector2f>{
            {position.x, position.y},
            {position.x + buttonSize.x - 2 * cornerRadius, position.y},
            {position.x, position.y + buttonSize.y - 2 * cornerRadius},
            {position.x + buttonSize.x - 2 * cornerRadius, position.y + buttonSize.y - 2 * cornerRadius}
            }) {
            corner.setPosition(pos);
            window.draw(corner);
        }

        window.draw(textObject);
        };

    // load map images from the assets folder 
    vector<Texture> mapTextures(4);
    bool texturesLoaded = true;
    for (int i = 0; i < 4; ++i) {
        if (!mapTextures[i].loadFromFile("assets/maps/map" + to_string(i + 1) + ".png")) {
            texturesLoaded = false;
            break;
        }
    }
    if (!texturesLoaded) {
        cout << "One or more map images failed to load!" << endl;
        window.close();
    }

    vector<Sprite> mapSprites(4);
    for (int i = 0; i < 4; ++i) {
        mapSprites[i].setTexture(mapTextures[i]);
        mapSprites[i].setScale(5.5f, 5.5f);  // Increase the size for better visibility
    }

    // map positioning for two rows (up and down)
    vector<Vector2f> mapPositions = {
        {100, 70}, // top-left
        {1200, 70}, // top-right
        {100, 550}, // bottom-left
        {1300, 550}  // bottom-right
    };

    // map names for buttons
    vector<string> mapNames = {
        "Desert", "Highway", "Winter", "Summer"
    };

    int selectedMapIndex = -1;

    // event loop to manage window interactions
    while (window.isOpen()) {
        Event event;
        bool isBackHovered = false;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            Vector2i mousePos = Mouse::getPosition(window);

            // map selection logic
            for (int i = 0; i < 4; ++i) {
                bool isHovered = mapSprites[i].getGlobalBounds().contains(Vector2f(mousePos));
                if (isHovered) {
                    mapSprites[i].setColor(Color::Yellow);
                    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                        selectedMapIndex = i;
                        selectedMapTexture = mapTextures[selectedMapIndex];
                    }
                }
                else {
                    mapSprites[i].setColor(Color::White);
                }
            }

            // "Confirm" button logic
            FloatRect backBounds(1980 / 2 - buttonSize.x / 2, 1080 - 100, buttonSize.x, buttonSize.y);
            if (backBounds.contains(Vector2f(mousePos))) {
                isBackHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    return;
                }
            }
        }

        Text backOption("Confirm", font, 50);
        window.clear();
        window.draw(menuTitlebackgroundTrackSprite);
        drawRoundedButton(Vector2f(1980 / 2 - buttonSize.x / 2, 1080 - 100), "Back", backOption, isBackHovered);

        // draw the map sprites at the new positions and add map names on top of each
        for (int i = 0; i < 4; ++i) {
            mapSprites[i].setPosition(mapPositions[i]);
            window.draw(mapSprites[i]);

            // create a button on top of each map on which the name of the ,map is written
            Text mapText(mapNames[i], font, 50);  // map name for each button
            mapText.setFillColor(Color::White);
            drawRoundedButton(Vector2f(mapPositions[i].x, mapPositions[i].y - 40), mapNames[i], mapText, false);  // Text button position
        }

        window.display();
    }
}











// this function will help the user to restart the game from the last checkpoint (called in tryAgain function)
void checkForCheckpoint(int& score) {
    //// If player crosses a checkpoint (e.g., 200, 400, etc.)
    //if (score >= lastCheckpointScore + 200) {
    //    lastCheckpointScore = score;  // Update the last checkpoint to the current score
    //    passedCheckpoint = true;      // Mark that the player passed a checkpoint
    //}
    switch (score / 200) {
    case 0:
        lastCheckpointScore = 0;
        break;
    case 1:
        lastCheckpointScore = 200;
        break;
    case 2:
        lastCheckpointScore = 400;
        break;
    case 3:
        lastCheckpointScore = 600;
        break;
    case 4:
        lastCheckpointScore = 800;
        break;
    case 5:
        lastCheckpointScore = 1000;
        break;

    }
    passedCheckpoint = true;

}



// this function will show the window of countdown 
void countDown(RenderWindow& window, Font& font) {
    int countDownTime = 3; // initialize the time with 3 
    Clock countDownClock;
    int targetCharacterSize = 100; // stregnth of the font 

    // create the countdown text object
    Text countDownText(to_string(countDownTime), font);
    countDownText.setFillColor(Color::Green);

    // initial position calculation (centered)
    countDownText.setPosition(window.getSize().x / 2 - countDownText.getLocalBounds().width / 2,
        window.getSize().y / 2 - countDownText.getLocalBounds().height / 2);
    // loading the countdown sound form the folder 
    SoundBuffer buffer;
    if (!buffer.loadFromFile(countDownSoundPath)) {
        cout << "Sound Not Found!";
        window.close();
    }

    Sound countDownSound(buffer);

    // play the countdown sound once
    countDownSound.play();

    while (countDownTime > 1) { 
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        // check if a second has passed
        if (countDownClock.getElapsedTime().asSeconds() >= 1.0f) {
            countDownTime--;
            countDownClock.restart();
        }

        // set text properties based on the countdown time
        if (countDownTime == 3) {
            countDownText.setFillColor(Color::Green);
            targetCharacterSize = 100;
        }
        else if (countDownTime == 2) {
            countDownText.setFillColor(Color::Yellow);
            targetCharacterSize = 150;
        }
        else if (countDownTime == 1) {
            countDownText.setFillColor(Color::Red);
            targetCharacterSize = 200;
        }

        // set the character size directly to the target size
        countDownText.setCharacterSize(targetCharacterSize);

        // recalculate position based on the new size beacuese every time a second the text strength increase so there is issue happening in its position
        countDownText.setPosition(window.getSize().x / 2 - countDownText.getLocalBounds().width / 2,
            window.getSize().y / 2 - countDownText.getLocalBounds().height / 2);

        // smoothly fade the text
        int alpha = (countDownTime * 255) / 3;
        countDownText.setFillColor(Color(countDownText.getFillColor().r, countDownText.getFillColor().g, countDownText.getFillColor().b, alpha));

        // update the text for the current countdown number
        countDownText.setString(to_string(countDownTime));

        // draw the countdown text
        window.clear();
        window.draw(countDownText);
        window.display();

        // wait briefly before the next frame to avoid double counting
        sleep(milliseconds(200));
    }

    // Fade out effect
    for (int i = 255; i >= 0; i -= 5) {
        window.clear();
        countDownText.setFillColor(Color(countDownText.getFillColor().r, countDownText.getFillColor().g, countDownText.getFillColor().b, i));
        window.draw(countDownText);
        window.display();
        sleep(milliseconds(10));
    }
}






// this fucntion is used to restart the game from the last checkpoint ( called in the tryAgain function)
void resetToCheckpoint(int& lives, int& score, float& currentFuel, Sprite& carSprite, const int trackWidth, const int trackHeight, bool& carSelected, vector<Sprite>& powerUps) {
    // If the player has passed a checkpoint it will  reset the game to the last checkpoint state
    if (passedCheckpoint) {
        score = lastCheckpointScore;  // Set score to the last checkpoint score
        lives = 10;                   // Reset lives (or any other values)
        currentFuel = 100.0f;         // Reset fuel
        carSprite.setPosition(trackWidth / 2, trackHeight - 120); // Reset car position
        carSelected = false;          // Reset car selection

        

        for (auto& powerUp : powerUps) {
            powerUp.setPosition(rand() % trackWidth, rand() % (trackHeight / 2)); // Reposition power-ups
        }

        passedCheckpoint = false;  // Reset the checkpoint flag
    }
}









// this function is used to create the rounded buttons in every other function i have created many buttons of same shape and color so i created a function to craete the button only by just passing the name of the button and their position
void drawRoundedButton(RenderWindow& window, Vector2f position, Vector2f size, const string& text, Font& font, bool isHovered, Color fillColor, Color hoverColor) {
    Color buttonColor = isHovered ? hoverColor : fillColor;

    // Draw base rectangle
    float cornerRadius = 20.0f;
    RectangleShape baseRect(Vector2f(size.x - 2 * cornerRadius, size.y));
    baseRect.setFillColor(buttonColor);
    baseRect.setPosition(position.x + cornerRadius, position.y);
    window.draw(baseRect);

    // Draw rounded corners
    CircleShape corner(cornerRadius);
    corner.setFillColor(buttonColor);

    corner.setPosition(position);
    window.draw(corner);

    corner.setPosition(position.x + size.x - 2 * cornerRadius, position.y);
    window.draw(corner);

    corner.setPosition(position.x, position.y + size.y - 2 * cornerRadius);
    window.draw(corner);

    corner.setPosition(position.x + size.x - 2 * cornerRadius, position.y + size.y - 2 * cornerRadius);
    window.draw(corner);

    // Draw text
    Text buttonText(text, font, 50);
    buttonText.setFillColor(Color::White);
    FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(
        position.x + (size.x - textBounds.width) / 2,
        position.y + (size.y - textBounds.height) / 2 - 10
    );
    window.draw(buttonText);
}













// function declartions 
void carSelection(RenderWindow& window, Font font, bool& carSelected, Texture& selectedCarTexture);
void menuSection(RenderWindow& window, Font& font, Texture& selectedCarTexture, bool isPaused, bool& carSelected, Music& backgroundMusic,int &score, int & lives);
void generateObstacles(vector<RectangleShape>& obstacles, int numObstacles, int obstacleSize, Vector2f trackBounds);
void settingsWindow(RenderWindow& window, Font& font, Texture& selectedCarTexture, bool& carSelected, bool ispaused, Music& backgroundMusic, int& score, int& lives);
int showPauseMenu(RenderWindow& window, Font& font, bool& ispaused, Texture selectedCarTexture, bool carSelected, Music& backgroundMusic,int &score,int &lives);
void showMessage(RenderWindow& window, Font& font);
void tryAgain(RenderWindow& window, Font& font, bool& isPaused, bool& carSelected, int& lives, int& score, float& currentFuel, vector<RectangleShape>& obstacles, Sprite& carSprite, const int trackWidth, const int trackHeight, Music& backgroundMusic, vector<CircleShape>& powerUps);
void resetGame(int& lives, int& score, float& currentFuel, vector<RectangleShape>& obstacles, Sprite& carSprite, const int trackWidth, const int trackHeight, bool& carSelected);
void managePowerUps(RenderWindow& window, Sprite& carSprite, vector<Sprite>& powerUps, Texture& fuelTexture, Texture& scoreTexture, Texture& livesTexture, float& currentFuel, int& score, int& speedBoostDuration, int& lives, Vector2f trackBounds);
void addProfile(RenderWindow& window, Font& font, Sound& clickSound);


// this function is used to create the obstacles ( called in the main function)
void generateObstacles(vector<Sprite>& obstacles, int numObstacles, Vector2f trackBounds, Texture &obstacleTexture) {
    obstacles.clear(); // clear existing obstacles
    srand(static_cast<unsigned>(time(0))); // initialize random seed

    // the image is already loaded and then pass to this function and used here

    for (int i = 0; i < numObstacles; ++i) {
        // create a sprite and assign the texture
        Sprite obstacle;
        obstacle.setTexture(obstacleTexture);

        // get the size of the texture to set positions properly
        FloatRect textureBounds = obstacle.getGlobalBounds();

        // position the obstacle within bounds so that they should appear only in the road 
        float randomX = 300 + rand() % (1600 - 300 - static_cast<int>(textureBounds.width)); // X within 300-1600
        float randomY = rand() % int(trackBounds.y / 2 - textureBounds.height);              // Y within the top half
        obstacle.setPosition(randomX, randomY);

        // add the sprite to the obstacles vector
        obstacles.push_back(obstacle);
    }
}



// this fucntion is used to create the diff profiles and these profiles are created in the profiles.txt 
void addProfile(RenderWindow& window, Font& font, Sound& clickSound) {
    // same as every fun used to create the buttons
    Vector2f buttonSize(200, 80);
    float cornerRadius = 20.0f; // Radius for rounded corners
    Color buttonFillColor = Color(70, 130, 180); // Steel blue for buttons
    Color hoverColor = Color(100, 149, 237); // Light steel blue for hover effect

    // set up text for entering the profile name
    Text profileInputText("Enter Profile Name: ", font, 50);
    profileInputText.setFillColor(Color::White);
    profileInputText.setPosition(500, 300);
    // initally there is no name
    string newProfileName = "";
    bool isProfileNameBeingEntered = true;

    // set up text for profile name input
    Text inputText("", font, 50);
    inputText.setFillColor(Color::White);
    inputText.setPosition(500, 400);

    // buttons Text
    Text okButtonText("OK", font, 40);
    Text cancelButtonText("Cancel", font, 40);

    // set up buttons position, colors
    RectangleShape okButton(Vector2f(buttonSize.x, buttonSize.y));
    okButton.setFillColor(buttonFillColor);
    okButton.setPosition(500, 500);

    RectangleShape cancelButton(Vector2f(buttonSize.x, buttonSize.y));
    cancelButton.setFillColor(buttonFillColor);
    cancelButton.setPosition(800, 500);

    // Helper function to draw rounded buttons
    auto drawRoundedButton = [&](RectangleShape& button, Text& buttonText, Vector2f position, bool isHovered) {
        Color fillColor = isHovered ? hoverColor : buttonFillColor;

        // Draw base rectangle with rounded corners
        RectangleShape baseRect(Vector2f(buttonSize.x - 2 * cornerRadius, buttonSize.y));
        baseRect.setFillColor(fillColor);
        baseRect.setPosition(position.x + cornerRadius, position.y);

        // Draw rounded corners
        CircleShape topLeftCorner(cornerRadius);
        topLeftCorner.setFillColor(fillColor);
        topLeftCorner.setPosition(position);

        CircleShape topRightCorner(cornerRadius);
        topRightCorner.setFillColor(fillColor);
        topRightCorner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y);

        CircleShape bottomLeftCorner(cornerRadius);
        bottomLeftCorner.setFillColor(fillColor);
        bottomLeftCorner.setPosition(position.x, position.y + buttonSize.y - 2 * cornerRadius);

        CircleShape bottomRightCorner(cornerRadius);
        bottomRightCorner.setFillColor(fillColor);
        bottomRightCorner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y + buttonSize.y - 2 * cornerRadius);

        // Draw corners and rectangle
        window.draw(topLeftCorner);
        window.draw(topRightCorner);
        window.draw(bottomLeftCorner);
        window.draw(bottomRightCorner);
        window.draw(baseRect);

        // Draw button text
        buttonText.setFillColor(Color::White);
        FloatRect textBounds = buttonText.getLocalBounds();
        buttonText.setPosition(position.x + (buttonSize.x - textBounds.width) / 2,
            position.y + (buttonSize.y - textBounds.height) / 2 - 10);
        window.draw(buttonText);
        };

    // Handle user input for the profile name
    while (isProfileNameBeingEntered) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }

            // Handle typing in the text box
            if (event.type == Event::TextEntered) {
                if (event.text.unicode == 8 && newProfileName.length() > 0) { // Backspace
                    newProfileName.pop_back();
                }
                else if (event.text.unicode < 128) {
                    newProfileName += static_cast<char>(event.text.unicode);
                }
            }

            // Check button clicks (OK and Cancel)
            Vector2i mousePos = Mouse::getPosition(window);

            // OK button click
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (okButton.getGlobalBounds().contains(Vector2f(mousePos))) {
                    // Save profile to file
                    ofstream profileFile("profiles.txt", ios::app);
                    profileFile << newProfileName << endl;
                    profileFile.close();

                    // Play sound and update message
                    clickSound.play();
                    profileInputText.setString("Profile Added: " + newProfileName);
                    isProfileNameBeingEntered = false;
                }
                // Cancel button click
                if (cancelButton.getGlobalBounds().contains(Vector2f(mousePos))) {
                    clickSound.play();
                    isProfileNameBeingEntered = false;  // Exit without saving
                }
            }
        }

        // Update the input text as the user types
        inputText.setString(newProfileName);

        // Draw UI
        window.clear();
        window.draw(profileInputText);
        window.draw(inputText);

        // Check hover effects for buttons
        bool isOkHovered = okButton.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window)));
        bool isCancelHovered = cancelButton.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window)));

        // Draw the rounded OK and Cancel buttons with hover effect
        drawRoundedButton(okButton, okButtonText, Vector2f(500, 500), isOkHovered);
        drawRoundedButton(cancelButton, cancelButtonText, Vector2f(800, 500), isCancelHovered);

        window.display();
    }
}



// this is used to save the playtime 
void savePlayTime(float playTime) {
    ofstream file("playtime.txt");
    if (file.is_open()) {
        file << playTime;
        file.close();
    }
}

// Function to load playtime from a file
float loadPlayTime() {
    ifstream file("playtime.txt");
    float playTime = 0.0f;
    if (file.is_open()) {
        file >> playTime;
        file.close();
    }
    return playTime;
}

// this function is used to create the power ups in the game (called in the main function)
void managePowerUps(RenderWindow& window, Sprite& carSprite, vector<Sprite>& powerUps, Texture& fuelTexture, Texture& scoreTexture, Texture& livesTexture, float& currentFuel, int& score, int& speedBoostDuration, int& lives, Vector2f trackBounds) {

    // generate power-ups based on score milestones (fuel will create after every 50 score)
    if (score % 50 == 0 && score != 0) {
        // add a fuel power-up if not already present
        if (std::none_of(powerUps.begin(), powerUps.end(), [&fuelTexture](Sprite& p) { return p.getTexture() == &fuelTexture; })) {
            Sprite fuelPowerUp;
            fuelPowerUp.setTexture(fuelTexture);
            fuelPowerUp.setPosition(300 + rand() % (1600 - 300 - static_cast<int>(fuelPowerUp.getGlobalBounds().width)), 0);
            fuelPowerUp.setScale(0.5f, 0.5f);
            powerUps.push_back(fuelPowerUp);
        }
    }
    // other two power ups create every 100 score
    if (score % 100 == 0 && score != 0) {
        // add a score power-up if not already present
        if (std::none_of(powerUps.begin(), powerUps.end(), [&scoreTexture](Sprite& p) { return p.getTexture() == &scoreTexture; })) {
            Sprite scorePowerUp;
            scorePowerUp.setTexture(scoreTexture);
            scorePowerUp.setPosition(300 + rand() % (1600 - 300 - static_cast<int>(scorePowerUp.getGlobalBounds().width)), 0);
            scorePowerUp.setScale(0.5f, 0.5f);
            powerUps.push_back(scorePowerUp);
        }

        // add a lives power-up if not already present
        if (std::none_of(powerUps.begin(), powerUps.end(), [&livesTexture](Sprite& p) { return p.getTexture() == &livesTexture; })) {
            Sprite livesPowerUp;
            livesPowerUp.setTexture(livesTexture);
            livesPowerUp.setPosition(300 + rand() % (1600 - 300 - static_cast<int>(livesPowerUp.getGlobalBounds().width)), 0);
            livesPowerUp.setScale(0.5f, 0.5f);
            powerUps.push_back(livesPowerUp);
        }
    }

    // Draw, move, and check for collisions with the car
    for (auto it = powerUps.begin(); it != powerUps.end();) {
        // Move the power-up downward
        it->move(0, 0.7f); // Adjust speed as needed

        // Remove power-up if it moves out of bounds
        if (it->getPosition().y > trackBounds.y) {
            it = powerUps.erase(it);
            continue;
        }

        window.draw(*it); // Render the power-up

        // Check for collision
        if (carSprite.getGlobalBounds().intersects(it->getGlobalBounds())) {
            // Determine the type of the power-up based on its texture
            if (it->getTexture() == &fuelTexture) {
                // Fuel Power-Up
                currentFuel = std::min(currentFuel + 20.0f, 100.0f); // Add fuel up to max
                cout << "Collected Power-Up: Fuel Refilled!\n";
            }
            else if (it->getTexture() == &scoreTexture) {
                // Score Power-Up
                score += 50;
                cout << "Collected Power-Up: Score Boost Activated!\n";
            }
            else if (it->getTexture() == &livesTexture) {
                // Lives Power-Up
                lives++;
                cout << "Collected Power-Up: Extra Life!\n";
            }

            it = powerUps.erase(it); // Remove collected power-up
        }
        else {
            ++it;
        }
    }

    // Handle Speed Boost Duration (if applicable)
    if (speedBoostDuration > 0) {
        --speedBoostDuration;
    }
}









// this fucntion is used to select the cars (called in the menuSection function)
void carSelection(RenderWindow& window, Font font, bool& carSelected, Texture& selectedCarTexture) {
    
    //creating boxes to add the cars in that boxes
    Vector2f buttonSize(400, 80);
    float cornerRadius = 20.0f;
    Color buttonFillColor = Color(70, 130, 180);
    Color hoverColor = Color(100, 149, 237);


    // sound of the click
    SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile(clickSound)) {
        cout << "Sound Not Found!";
        window.close();
    }
    Sound clickSound(clickBuffer);

    // background texture
    Texture menuTitlebackgroundTexture;
    if (!menuTitlebackgroundTexture.loadFromFile(menuTitleImage)) {
        cout << "Image not found!" << endl;
        window.close();
    }

    Sprite menuTitlebackgroundTrackSprite;
    menuTitlebackgroundTrackSprite.setTexture(menuTitlebackgroundTexture);
    menuTitlebackgroundTrackSprite.setScale(
        float(1980) / menuTitlebackgroundTexture.getSize().x,
        float(1080) / menuTitlebackgroundTexture.getSize().y
    );

    // Helper function to draw rounded buttons
    auto drawRoundedButton = [&](Vector2f position, const string& text, Text& textObject, bool isHovered) {
        Color fillColor = isHovered ? hoverColor : buttonFillColor;

        // Base rectangle
        RectangleShape baseRect(Vector2f(buttonSize.x - 2 * cornerRadius, buttonSize.y));
        baseRect.setFillColor(fillColor);
        baseRect.setPosition(position.x + cornerRadius, position.y);

        // Circles for corners
        CircleShape corner(cornerRadius);
        corner.setFillColor(fillColor);

        // Text
        textObject.setString(text);
        textObject.setCharacterSize(50);
        textObject.setFillColor(Color::White);
        FloatRect textBounds = textObject.getLocalBounds();
        textObject.setPosition(
            position.x + (buttonSize.x - textBounds.width) / 2,
            position.y + (buttonSize.y - textBounds.height) / 2 - 10
        );

        // Draw components
        window.draw(baseRect);

        // Draw corners
        for (const auto& pos : vector<Vector2f>{
                {position.x, position.y},
                {position.x + buttonSize.x - 2 * cornerRadius, position.y},
                {position.x, position.y + buttonSize.y - 2 * cornerRadius},
                {position.x + buttonSize.x - 2 * cornerRadius, position.y + buttonSize.y - 2 * cornerRadius}
            }) {
            corner.setPosition(pos);
            window.draw(corner);
        }

        window.draw(textObject);
        };

    // Load car images for selection
    vector<Texture> carTextures(6);
    bool texturesLoaded = true;
    for (int i = 0; i < 6; ++i) {
        if (!carTextures[i].loadFromFile("assets/cars/Image" + to_string(i + 1) + ".png")) {
            texturesLoaded = false;
            break;
        }
    }
    if (!texturesLoaded) {
        cout << "One or more car images failed to load!" << endl;
        window.close();
    }
    // this will show the all 6 images in the diff boxes
    vector<Sprite> carSprites(6);
    for (int i = 0; i < 6; ++i) {
        carSprites[i].setTexture(carTextures[i]);
        carSprites[i].setScale(2.5f, 2.5f);
        carSprites[i].setPosition(135 + i * 300, 1080 / 2 - 120);
    }
    // this will craete the boxes
    vector<RectangleShape> carBoxes(6);
    for (int i = 0; i < 6; ++i) {
        carBoxes[i].setSize(Vector2f(250, 150));
        carBoxes[i].setFillColor(Color(70, 130, 180, 180));
        carBoxes[i].setOutlineColor(Color::White);
        carBoxes[i].setOutlineThickness(5);
        carBoxes[i].setPosition(100 + i * 300, 1080 / 2 - 120);
    }

    int selectedCarIndex = -1;

    // Function to draw the car selection box with rounded corners
    auto drawCarBox = [&](int i, bool isHovered, bool isSelected) {
        RectangleShape& carBox = carBoxes[i];

        // Highlight on hover
        carBox.setFillColor(isHovered ? Color(135, 206, 250) : Color(70, 130, 180, 180));

        // Outline glow effect when selected
        carBox.setOutlineColor(isSelected ? Color(255, 215, 0) : Color::White);
        carBox.setOutlineThickness(isSelected ? 7 : 5);

        // Draw shadow
        RectangleShape shadow(Vector2f(250, 150));
        shadow.setFillColor(Color(0, 0, 0, 100));
        shadow.setPosition(carBox.getPosition().x + 5, carBox.getPosition().y + 5);
        window.draw(shadow);

        // Draw box and sprite
        window.draw(carBox);
        window.draw(carSprites[i]);
        };

    // Event loop to manage window interactions
    while (window.isOpen()) {
        Event event;
        bool isBackHovered = false;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            Vector2i mousePos = Mouse::getPosition(window);

            // Car selection logic
            for (int i = 0; i < 6; ++i) {
                bool isHovered = carBoxes[i].getGlobalBounds().contains(Vector2f(mousePos));
                if (isHovered) {
                    carSprites[i].setColor(Color::Yellow);
                    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                        selectedCarIndex = i;
                        selectedCarTexture = carTextures[selectedCarIndex];
                        carSelected = true;
                    }
                }
                else {
                    carSprites[i].setColor(Color::White);
                }

                // Draw car box
                drawCarBox(i, isHovered, selectedCarIndex == i);
            }

            // "Back to Menu" button logic
            FloatRect backBounds(1980 / 2 - buttonSize.x / 2, 1080 - 100, buttonSize.x, buttonSize.y);
            if (backBounds.contains(Vector2f(mousePos))) {
                isBackHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    return;
                }
            }
        }
        Text backOption("Back", font, 50);
        window.clear();
        window.draw(menuTitlebackgroundTrackSprite);
        drawRoundedButton(Vector2f(1980 / 2 - buttonSize.x / 2, 1080 - 100), "Back", backOption, isBackHovered);

        // Display all the car boxes and sprites
        for (int i = 0; i < 6; ++i) {
            drawCarBox(i, false, selectedCarIndex == i);
        }

        window.display();
    }
}


// this function will used when user is want to go back to the menu then this function will completely reset the game the car selected map every thing (called in the tryAgain)
void resetGame(int& lives, int& score, float& currentFuel, vector<RectangleShape>& obstacles, Sprite& carSprite, const int trackWidth, const int trackHeight, bool& carSelected, vector<Sprite>& powerUps, float& totalTime, Texture& selectedCarTexture) {
    // Reset game variables
    lives = 10; // Reset lives
    score = 0; // Reset score
    currentFuel = 100.0f; // Reset fuel
    carSprite.setPosition(trackWidth / 2, trackHeight - 120); // Reset car position
    carSelected = false; // Reset car selection
    totalTime = 0.0f;
    // Properly reset the selected car texture
    selectedCarTexture = sf::Texture(); // Reinitialize the texture to a default state
    carSprite.setTexture(selectedCarTexture); // Detach the texture from the sprite

    // Reset obstacles - reposition randomly within the screen bounds
    for (auto& obstacle : obstacles) {
        obstacle.setPosition(rand() % trackWidth, rand() % (trackHeight / 2)); // Place obstacles at random positions within upper half of the track
    }

    // Reset power-ups - reposition them randomly
    for (auto& powerUp : powerUps) {
        powerUp.setPosition(rand() % trackWidth, rand() % (trackHeight / 2)); // Place power-ups at random positions
    }

    
}




// this function will only reset the specific things like score , lives and fuel etc .. (called in the tryAgain function)
void restartGame(int& lives, int& score, float& currentFuel, Sprite& carSprite, const int trackWidth, const int trackHeight, bool& carSelected, vector<Sprite>& powerUps, float& totalTime, Texture& selectedCarTexture, float& previousPlayTime, Clock& gameClock) {
    // Reset game variables
    lives = 10; // Reset lives
    score = 0; // Reset score
    currentFuel = 100.0f; // Reset fuel
    carSprite.setPosition(trackWidth / 2, trackHeight - 120); // Reset car position
    carSelected = false; // Reset car selection
    totalTime = -3.0f;
    previousPlayTime = -3.0f;
    gameClock.restart();
    //// Properly reset the selected car texture
    //selectedCarTexture = sf::Texture(); // Reinitialize the texture to a default state
    //carSprite.setTexture(selectedCarTexture); // Detach the texture from the sprite

    

    // Reset power-ups - reposition them randomly
    for (auto& powerUp : powerUps) {
        powerUp.setPosition(rand() % trackWidth, rand() % (trackHeight / 2)); // Place power-ups at random positions
    }

    // Optionally, you can also reset other things, like power-up states or durations, if applicable.
}




// this function is used to show the user the the final score , playtime, cause of lose of game and other options like restart the game etc ( called after losing game either by losing due to score or fuel)
void tryAgain(RenderWindow& window, Font& font, bool& isPaused, bool& carSelected, int& lives, int& score, float& currentFuel, Sprite& carSprite, const int trackWidth, const int trackHeight, Music& backgroundMusic, vector<Sprite>& powerUps, float& totalTime, Texture& selectedCarTexture, float& previousPlayTime, Clock& gameClock) {
    // same as every function to create the specific buttons
    
    Vector2f buttonSize(400, 80); // Button Size
    float cornerRadius = 20.0f;   // Radius for rounded corners
    Color buttonFillColor = Color(70, 130, 180); // Steel blue for buttons
    Color hoverColor = Color(100, 149, 237);     // Light steel blue for hover effect

    // sound of button click
    SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile(clickSound)) {
        cout << "Sound Not Found!";
        window.close();
    }
    Sound clickSound(clickBuffer);

    // Text for buttons
    Text tryAgainText("Try Again", font, 30);
    Text mainMenuText("Main Menu", font, 30);
    Text exitText("Exit", font, 30);
    Text lastCheckpointText("Last Checkpoint", font, 30);

    // Total Time Text
    ostringstream timeStream;
    timeStream << fixed << setprecision(1) << totalTime << "s";
    Text timeText("Total Time: " + timeStream.str(), font, 40);
    timeText.setFillColor(Color::White);

    // Helper function to draw rounded buttons with hover effects
    auto drawRoundedButton = [&](Vector2f position, const std::string& text, Text& textObject, bool isHovered) {
        Color fillColor = isHovered ? hoverColor : buttonFillColor;
        float scale = isHovered ? 1.05f : 1.0f; // Slight scaling on hover

        // Base rectangle
        RectangleShape baseRect(Vector2f(buttonSize.x - 2 * cornerRadius, buttonSize.y));
        baseRect.setFillColor(fillColor);
        baseRect.setPosition(position.x + cornerRadius, position.y);
        baseRect.setScale(scale, scale);

        // Circles for corners
        CircleShape corner(cornerRadius);
        corner.setFillColor(fillColor);

        // Text
        textObject.setString(text);
        textObject.setCharacterSize(40);
        textObject.setFillColor(Color::White);
        FloatRect textBounds = textObject.getLocalBounds();
        textObject.setPosition(
            position.x + (buttonSize.x - textBounds.width) / 2,
            position.y + (buttonSize.y - textBounds.height) / 2 - 10
        );

        // Draw components
        window.draw(baseRect);

        // Draw corner circles
        corner.setPosition(position);
        window.draw(corner); // Top-left
        corner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y);
        window.draw(corner); // Top-right
        corner.setPosition(position.x, position.y + buttonSize.y - 2 * cornerRadius);
        window.draw(corner); // Bottom-left
        corner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y + buttonSize.y - 2 * cornerRadius);
        window.draw(corner); // Bottom-right

        // Draw text
        window.draw(textObject);
        };

    // Layout positions
    float centerX = window.getSize().x / 2 - buttonSize.x / 2; // Center horizontally
    float startY = 400; // Starting Y position for buttons

    // Game Over and Final Score Texts
    Text gameOverText("Game Over!", font, 80);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2, 80);

    Text finalScoreText("Final Score: " + to_string(score), font, 50);
    finalScoreText.setFillColor(Color::White);
    finalScoreText.setPosition(window.getSize().x / 2 - finalScoreText.getGlobalBounds().width / 2, 180);

    Text lossReasonText("", font, 40);
    lossReasonText.setFillColor(Color::Yellow);

    // this will check that if user lose due to either score or fuel then it will show the cause of lose
    if (currentFuel == 0) {
        lossReasonText.setString("Out of Fuel!");
    }
    else if (lives == 0) {
        lossReasonText.setString("No Lives Left!");
    }
    lossReasonText.setPosition(window.getSize().x / 2 - lossReasonText.getGlobalBounds().width / 2, 250);

    timeText.setPosition(window.getSize().x / 2 - timeText.getGlobalBounds().width / 2, 310);

    // Background Overlay
    RectangleShape backgroundOverlay(Vector2f(window.getSize().x, window.getSize().y));
    backgroundOverlay.setFillColor(Color(0, 0, 0, 150));

    while (window.isOpen()) {
        bool isTryAgainHovered = false, isMainMenuHovered = false, isExitHovered = false, isLastCheckpointHovered = false;

        Event pauseEvent;
        while (window.pollEvent(pauseEvent)) {
            if (pauseEvent.type == Event::Closed) window.close();

            Vector2i mousePos = Mouse::getPosition(window);

            // Buttons hover and click detection
            FloatRect lastCheckpointBounds(centerX, startY, buttonSize.x, buttonSize.y);
            FloatRect tryAgainBounds(centerX, startY + 90, buttonSize.x, buttonSize.y);
            FloatRect mainMenuBounds(centerX, startY + 180, buttonSize.x, buttonSize.y);
            FloatRect exitBounds(centerX, startY + 270, buttonSize.x, buttonSize.y);

            if (lastCheckpointBounds.contains(Vector2f(mousePos))) {
                isLastCheckpointHovered = true;
                if (pauseEvent.type == Event::MouseButtonPressed && pauseEvent.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    resetToCheckpoint(lives, score, currentFuel, carSprite, trackWidth, trackHeight, carSelected, powerUps);
                    countDown(window, font);
                    return;
                }
            }
            if (tryAgainBounds.contains(Vector2f(mousePos))) {
                isTryAgainHovered = true;
                if (pauseEvent.type == Event::MouseButtonPressed && pauseEvent.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    isPaused = false;
                    // Save the playtime and reset the timer
                    previousPlayTime += totalTime;
                    savePlayTime(previousPlayTime);
                    totalTime = -3.0f;
                    restartGame(lives, score, currentFuel, carSprite, trackWidth, trackHeight, carSelected, powerUps, totalTime, selectedCarTexture, previousPlayTime, gameClock);
                    countDown(window, font);
                    return;
                }
            }
            if (mainMenuBounds.contains(Vector2f(mousePos))) {
                isMainMenuHovered = true;
                if (pauseEvent.type == Event::MouseButtonPressed && pauseEvent.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    restartGame(lives, score, currentFuel, carSprite, trackWidth, trackHeight, carSelected, powerUps, totalTime, selectedCarTexture, previousPlayTime, gameClock);
                    menuSection(window, font, selectedCarTexture, isPaused, carSelected, backgroundMusic,score,lives);
                    return;
                }
            }
            if (exitBounds.contains(Vector2f(mousePos))) {
                isExitHovered = true;
                if (pauseEvent.type == Event::MouseButtonPressed && pauseEvent.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    window.close();
                }
            }
        }

        // Draw everything
        window.clear();
        window.draw(backgroundOverlay);

        window.draw(gameOverText);
        window.draw(finalScoreText);
        window.draw(lossReasonText);
        window.draw(timeText);

        drawRoundedButton(Vector2f(centerX, startY), "Last Checkpoint", lastCheckpointText, isLastCheckpointHovered);
        drawRoundedButton(Vector2f(centerX, startY + 90), "Try Again", tryAgainText, isTryAgainHovered);
        drawRoundedButton(Vector2f(centerX, startY + 180), "Main Menu", mainMenuText, isMainMenuHovered);
        drawRoundedButton(Vector2f(centerX, startY + 270), "Exit", exitText, isExitHovered);

        window.display();
    }
}





// this function will show the message to select the car before starting game 
void showChooseCarMessage(RenderWindow& window, Font& font, bool& carSelected) {
    // Semi-transparent black background for better visibility of the text
    RectangleShape background(Vector2f(window.getSize()));
    background.setFillColor(Color(0, 0, 0, 200));

    // sound of button click
    SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile(clickSound)) {
        cout << "Sound Not Found!";
        window.close();
    }
    Sound clickSound(clickBuffer);

    // Message text with scaling animation
    Text message("Please select a car from the Car Selection before starting!", font, 30);
    message.setFillColor(Color::White);
    message.setStyle(Text::Bold);
    message.setOrigin(message.getLocalBounds().width / 2, message.getLocalBounds().height / 2);
    message.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 150);

    // Animation variables for message scaling
    float scale = 0.7f;
    bool scalingUp = true;

    // Helper function to draw rounded buttons with hover effect
    auto drawRoundedButton = [&](Vector2f position, const std::string& text, Text& textObject, bool isHovered) {
        Color buttonColor = Color(70, 130, 180);     // Default color
        Color hoverColor = Color(100, 149, 237);     // Hover color
        float width = 400, height = 80, cornerRadius = height / 2;

        // Draw shadow
        Color shadowColor = Color(0, 0, 0, 100);  // Shadow color
        RectangleShape shadow(Vector2f(width - 2 * cornerRadius, height));
        shadow.setFillColor(shadowColor);
        shadow.setPosition(position.x + cornerRadius + 5, position.y + 5);
        CircleShape shadowCorner(cornerRadius);
        shadowCorner.setFillColor(shadowColor);
        shadowCorner.setPosition(position.x + 5, position.y + 5);
        window.draw(shadow);
        window.draw(shadowCorner);
        shadowCorner.setPosition(position.x + width - 2 * cornerRadius + 5, position.y + 5);
        window.draw(shadowCorner);

        // Button
        Color fillColor = isHovered ? hoverColor : buttonColor;
        RectangleShape button(Vector2f(width - 2 * cornerRadius, height));
        button.setFillColor(fillColor);
        button.setPosition(position.x + cornerRadius, position.y);
        CircleShape leftCorner(cornerRadius), rightCorner(cornerRadius);
        leftCorner.setFillColor(fillColor);
        rightCorner.setFillColor(fillColor);
        leftCorner.setPosition(position.x, position.y);
        rightCorner.setPosition(position.x + width - 2 * cornerRadius, position.y);
        window.draw(button);
        window.draw(leftCorner);
        window.draw(rightCorner);

        // Button text
        textObject.setString(text);
        textObject.setCharacterSize(40);
        textObject.setFillColor(Color::White);
        textObject.setOrigin(textObject.getLocalBounds().width / 2, textObject.getLocalBounds().height / 2);
        textObject.setPosition(position.x + width / 2, position.y + height / 2 - 5);
        window.draw(textObject);
        };

    // "OK" button text
    Text okText("OK", font, 40);
    bool isOkButtonHovered = false;

    // Main loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }

            // Check for OK button hover and click
            Vector2i mousePos = Mouse::getPosition(window);
            FloatRect okBounds(window.getSize().x / 2 - 200, window.getSize().y / 2 + 50, 400, 80);
            if (okBounds.contains(Vector2f(mousePos))) {
                isOkButtonHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    return;  // Close the message
                }
            }
            else {
                isOkButtonHovered = false;
            }
        }

        // Scale the message text up and down
        scale += (scalingUp ? 0.002f : -0.002f);
        if (scale >= 1.0f) scalingUp = false;
        if (scale <= 0.9f) scalingUp = true;
        message.setScale(scale, scale);

        // Clear the window and draw elements
        window.clear();
        window.draw(background); // Background overlay
        window.draw(message);    // Message with scaling animation

        // Draw the "OK" button with hover effect
        drawRoundedButton(Vector2f(window.getSize().x / 2 - 200, window.getSize().y / 2 + 50), "OK", okText, isOkButtonHovered);

        window.display(); // Update the window
    }
}






// this function will show the different options to user to either start setting and car selection etc (called in the main function)
void menuSection(RenderWindow& window, Font& font, Texture& selectedCarTexture, bool ispaused, bool& carSelected, Music& backgroundMusic,int& score,int& lives) {
    
    // same as every function to create the buttons
    Vector2f buttonSize(400, 80);
    float cornerRadius = 20.0f; // Radius for rounded corners
    Color buttonFillColor = Color(70, 130, 180); // Steel blue for buttons
    Color hoverColor = Color(100, 149, 237); // Light steel blue for hover effect

    SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile(clickSound)) {
        cout << "Sound Not Found!";
        window.close();
    }
    Sound clickSound(clickBuffer);


    // loading the background image 
    Texture menuTitlebackgroundTexture;
    if (!menuTitlebackgroundTexture.loadFromFile(menuTitleImage)) {
        cout << "Image not found!" << endl;
        window.close();
    }

    Sprite menuTitlebackgroundTrackSprite;
    menuTitlebackgroundTrackSprite.setTexture(menuTitlebackgroundTexture);
    menuTitlebackgroundTrackSprite.setScale(
        float(1980) / menuTitlebackgroundTexture.getSize().x,
        float(1080) / menuTitlebackgroundTexture.getSize().y
    );

    // Helper function to draw rounded buttons
    auto drawRoundedButton = [&](Vector2f position, const std::string& text, Text& textObject, bool isHovered) {
        Color fillColor = isHovered ? hoverColor : buttonFillColor;

        // Base rectangle
        RectangleShape baseRect(Vector2f(buttonSize.x - 2 * cornerRadius, buttonSize.y));
        baseRect.setFillColor(fillColor);
        baseRect.setPosition(position.x + cornerRadius, position.y);

        // Circles for corners
        CircleShape corner(cornerRadius);
        corner.setFillColor(fillColor);

        // Text
        textObject.setString(text);
        textObject.setCharacterSize(50);
        textObject.setFillColor(Color::White);
        FloatRect textBounds = textObject.getLocalBounds();
        textObject.setPosition(
            position.x + (buttonSize.x - textBounds.width) / 2,
            position.y + (buttonSize.y - textBounds.height) / 2 - 10
        );

        // Draw components
        window.draw(baseRect);

        // Draw four corners
        corner.setPosition(position);
        window.draw(corner);
        corner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y);
        window.draw(corner);
        corner.setPosition(position.x, position.y + buttonSize.y - 2 * cornerRadius);
        window.draw(corner);
        corner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y + buttonSize.y - 2 * cornerRadius);
        window.draw(corner);

        // Draw text
        window.draw(textObject);
        };

    // Create button text objects
    Text startOption("", font), settingOption("", font), quitOption("", font), carSelectionOption("", font);
    bool isStartHovered = false, isSettingHovered = false, isQuitHovered = false, isCarSelectionHovered = false; // initially there is no hover effect

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            Vector2i mousePos = Mouse::getPosition(window);

            // Handle Start Button hover and click
            FloatRect startBounds(130, 200, buttonSize.x, buttonSize.y);
            if (startBounds.contains(Vector2f(mousePos))) {
                isStartHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (!carSelected) {
                        clickSound.play();
                        showChooseCarMessage(window, font, carSelected);
                    }
                    else {
                        mapSelection(window, font, selectedMapTexture);
                        countDown(window, font);
                        return; // Start the game
                    }
                }
            }
            else {
                isStartHovered = false;
            }

            // Handle Car Selection Button hover and click
            FloatRect carSelectionBounds(130, 300, buttonSize.x, buttonSize.y);
            if (carSelectionBounds.contains(Vector2f(mousePos))) {
                isCarSelectionHovered = true;// whenever the cursor appear in this boundry then hover effet will be showen
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    carSelection(window, font, carSelected, selectedCarTexture); // open the carSelection window to select cars
                }
            }
            else {
                isCarSelectionHovered = false;
            }

            // Handle Settings Button hover and click
            FloatRect settingBounds(130, 400, buttonSize.x, buttonSize.y);
            if (settingBounds.contains(Vector2f(mousePos))) {
                isSettingHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    settingsWindow(window, font, selectedCarTexture, carSelected, ispaused, backgroundMusic,score,lives); // opening setting
                }
            }
            else {
                isSettingHovered = false;
            }

            // Handle Quit Button hover and click
            FloatRect quitBounds(130, 500, buttonSize.x, buttonSize.y);
            if (quitBounds.contains(Vector2f(mousePos))) {
                isQuitHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    window.close(); // closing game
                }
            }
            else {
                isQuitHovered = false;
            }
        }

        // Clear window and draw background
        window.clear();
        window.draw(menuTitlebackgroundTrackSprite);

        // Draw buttons with hover effects
        drawRoundedButton(Vector2f(130, 200), "Start Game", startOption, isStartHovered);
        drawRoundedButton(Vector2f(130, 300), "Car Selection", carSelectionOption, isCarSelectionHovered);
        drawRoundedButton(Vector2f(130, 400), "Settings", settingOption, isSettingHovered);
        drawRoundedButton(Vector2f(130, 500), "Quit Game", quitOption, isQuitHovered);

        window.display();
    }
}



// this function is used to control the different game states like music screen etc .. (called in the mainMenu fucntion)
void settingsWindow(RenderWindow& window, Font& font, Texture& selectedCarTexture, bool& carSelected, bool ispaused, Music& backgroundMusic,int &score,int &lives) {
    
    // same as every function to create the buttons
    Vector2f buttonSize(400, 80);
    Vector2f checkboxSize(30, 30);
    float cornerRadius = 20.0f;
    Color buttonFillColor = Color(70, 130, 180); // Steel blue
    Color hoverColor = Color(100, 149, 237); // Light steel blue
    Color checkboxColor = Color(255, 255, 255); // White
    Color checkboxCheckedColor = Color(0, 255, 0); // Green
    SoundBuffer clickBuffer;


    if (!clickBuffer.loadFromFile(clickSound)) {
        cout << "Sound Not Found!" << endl;
        window.close();
    }
    Sound clickSound(clickBuffer);

    // Background texture
    Texture menuTitlebackgroundTexture;
    if (!menuTitlebackgroundTexture.loadFromFile(menuTitleImage)) {
        cout << "Image not found!" << endl;
        window.close();
    }

    Sprite menuTitlebackgroundTrackSprite;
    menuTitlebackgroundTrackSprite.setTexture(menuTitlebackgroundTexture);
    menuTitlebackgroundTrackSprite.setScale(float(1980) / menuTitlebackgroundTexture.getSize().x,
        float(1080) / menuTitlebackgroundTexture.getSize().y);

    // Fullscreen state variable
    bool isFullscreen = false;

    // Helper function to draw rounded buttons
    auto drawRoundedButton = [&](Vector2f position, const string& text, Text& textObject, bool isHovered) {
        Color fillColor = isHovered ? hoverColor : buttonFillColor;
        RectangleShape baseRect(Vector2f(buttonSize.x - 2 * cornerRadius, buttonSize.y));
        baseRect.setFillColor(fillColor);
        baseRect.setPosition(position.x + cornerRadius, position.y);
        CircleShape corner(cornerRadius);
        corner.setFillColor(fillColor);

        // Text
        textObject.setString(text);
        textObject.setCharacterSize(50);
        textObject.setFillColor(Color::White);
        FloatRect textBounds = textObject.getLocalBounds();
        textObject.setPosition(position.x + (buttonSize.x - textBounds.width) / 2,
            position.y + (buttonSize.y - textBounds.height) / 2 - 10);

        // Draw components
        window.draw(baseRect);

        corner.setPosition(position);
        window.draw(corner);
        corner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y);
        window.draw(corner);
        corner.setPosition(position.x, position.y + buttonSize.y - 2 * cornerRadius);
        window.draw(corner);
        corner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y + buttonSize.y - 2 * cornerRadius);
        window.draw(corner);

        window.draw(textObject);
        };

    // Initialize text objects
    Text backOption("", font), musicOption("", font), fullscreenOption("", font), saveOption("", font), loadOption("", font), addProfileOption("", font);

    bool isMusicPaused = true;
    bool isCheckboxChecked = false;

    // Volume control slider
    RectangleShape volumeBar(Vector2f(300, 20));
    volumeBar.setFillColor(Color(169, 169, 169));
    volumeBar.setPosition(1980 / 2 - volumeBar.getSize().x / 2, 1080 - 400);

    CircleShape volumeHandle(10);
    volumeHandle.setFillColor(Color::Green);
    volumeHandle.setPosition(1980 / 2 - volumeBar.getSize().x / 2 + 150, 1080 - 400);

    float volume = 50;
    backgroundMusic.setVolume(volume);
    // text of volume 
    Text volumeText("Volume", font, 50);
    volumeText.setFillColor(Color::White);
    volumeText.setPosition(1980 / 2 - volumeText.getLocalBounds().width / 2, 1080 - 470);


    bool isDraggingVolumeHandle = false;

    while (window.isOpen()) {
        Event event;
        bool isBackHovered = false, isSaveHovered = false, isLoadHovered = false, isAddProfileHovered = false;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            Vector2i mousePos = Mouse::getPosition(window);

            // "Back to Menu" button logic
            FloatRect backBounds(1980 / 2 - buttonSize.x / 2, 1080 - 150, buttonSize.x, buttonSize.y);
            if (backBounds.contains(Vector2f(mousePos))) {
                isBackHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    return;
                }
            }

            // "Save" button logic
            FloatRect saveBounds(100, 400, buttonSize.x, buttonSize.y);
            if (saveBounds.contains(Vector2f(mousePos))) {
                isSaveHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    saveGame(score, lives);
                }
            }

            // "Load" button logic
            FloatRect loadBounds(100, 500, buttonSize.x, buttonSize.y);
            if (loadBounds.contains(Vector2f(mousePos))) {
                isLoadHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    // Load the saved settings from the file
                    loadGame(score, lives);
                }
            }

            // "Add Profile" button logic
            FloatRect addProfileBounds(100, 600, buttonSize.x, buttonSize.y);
            if (addProfileBounds.contains(Vector2f(mousePos))) {
                isAddProfileHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    // Call the addProfile function
                    addProfile(window, font, clickSound);
                }
            }

            // Music control checkbox logic
            FloatRect checkboxBounds(1980 / 2 - buttonSize.x / 2 - 20, 1080 - 339, checkboxSize.x, checkboxSize.y);
            if (checkboxBounds.contains(Vector2f(mousePos))) {
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    isCheckboxChecked = !isCheckboxChecked;
                    if (isCheckboxChecked) {
                        backgroundMusic.pause();
                        isMusicPaused = true;
                    }
                    else {
                        backgroundMusic.play();
                        isMusicPaused = false;
                    }
                }
            }

            // Fullscreen toggle logic
            FloatRect fullscreenBounds(1980 / 2 - buttonSize.x / 2, 1080 - 270, buttonSize.x, buttonSize.y);
            if (fullscreenBounds.contains(Vector2f(mousePos))) {
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    isFullscreen = !isFullscreen;
                    if (isFullscreen) {
                        window.create(VideoMode(1920, 1080), "Racing Game", Style::Fullscreen);
                    }
                    else {
                        window.create(VideoMode(1920, 1080), "Racing Game", Style::Default);
                    }
                }
            }

            // Volume control logic (dragging slider functionality)
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (volumeHandle.getGlobalBounds().contains(Vector2f(mousePos))) {
                    isDraggingVolumeHandle = true;
                }
            }

            if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
                isDraggingVolumeHandle = false;
            }

            if (isDraggingVolumeHandle) {
                // Update handle position based on mouse drag
                float newPos = min(max(float(mousePos.x - volumeBar.getPosition().x), 0.f), volumeBar.getSize().x);
                volumeHandle.setPosition(volumeBar.getPosition().x + newPos - volumeHandle.getRadius(), volumeHandle.getPosition().y);

                // Update volume based on handle position
                volume = newPos / volumeBar.getSize().x * 100.f;
                backgroundMusic.setVolume(volume);
            }
        }

        window.clear();
        window.draw(menuTitlebackgroundTrackSprite);

        // Draw buttons and controls
        drawRoundedButton(Vector2f(1980 / 2 - buttonSize.x / 2, 1080 - 150), "Back to Menu", backOption, isBackHovered);
        drawRoundedButton(Vector2f(100, 400), "Save", saveOption, isSaveHovered);
        drawRoundedButton(Vector2f(100, 500), "Load", loadOption, isLoadHovered);
        drawRoundedButton(Vector2f(100, 600), "Add Profile", addProfileOption, isAddProfileHovered);
        drawRoundedButton(Vector2f(1980 / 2 - buttonSize.x / 2, 1080 - 270), "Fullscreen", fullscreenOption, isFullscreen);

        // Draw the volume slider and volume text
        window.draw(volumeBar);
        window.draw(volumeHandle);
        window.draw(volumeText);

        // Draw the music checkbox and label
        if (isCheckboxChecked) {
            RectangleShape checkboxRect(checkboxSize);
            checkboxRect.setFillColor(checkboxCheckedColor);
            checkboxRect.setPosition(1980 / 2 - buttonSize.x / 2 - 20, 1080 - 339);
            window.draw(checkboxRect);
        }
        else {
            RectangleShape checkboxRect(checkboxSize);
            checkboxRect.setOutlineThickness(3);
            checkboxRect.setOutlineColor(checkboxColor);
            checkboxRect.setFillColor(Color::Transparent);
            checkboxRect.setPosition(1980 / 2 - buttonSize.x / 2 - 20, 1080 - 339);
            window.draw(checkboxRect);
        }

        // Draw "Music On/Off" text
        Text musicText(isMusicPaused ? "Music Off" : "Music On", font, 60);
        musicText.setFillColor(Color::Red);
        musicText.setPosition(1980 / 2 - musicText.getLocalBounds().width / 2 - 10, 1080 - 350);
        window.draw(musicText);

        window.display();
    }
}



// this function is used to pause the game at any moment and show the diff buttons like resume, setting etc( called in main function)
int showPauseMenu(RenderWindow& window, Font& font, bool& ispaused, Texture selectedCarTexture, bool carSelected, Music& backgroundMusic,int &score, int &lives) {
    // same as every function to create buttons
    Vector2f buttonSize(400, 80);
    float cornerRadius = 20.0f; // Radius for rounded corners
    Color buttonFillColor = Color(70, 130, 180); // Steel blue for buttons
    Color hoverColor = Color(100, 149, 237); // Light steel blue for hover effect

    SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile(clickSound)) {
        cout << "Sound Not Found!" << endl;
        window.close();
    }
    Sound clickSound(clickBuffer);

    // Load pause background texture
    Texture pauseTitleBackgroundTexture;
    if (!pauseTitleBackgroundTexture.loadFromFile(pauseTitleImage)) {
        cout << "Image Not Found!" << endl;
        window.close();
    }

    Sprite pauseTitleBackgroundSprite;
    pauseTitleBackgroundSprite.setTexture(pauseTitleBackgroundTexture);
    pauseTitleBackgroundSprite.setScale(
        float(1980) / pauseTitleBackgroundTexture.getSize().x,
        float(1080) / pauseTitleBackgroundTexture.getSize().y
    );

    // Text for buttons
    Text resumeText("Resume", font, 50);
    Text settingsText("Settings", font, 50);
    Text mainMenuText("Main Menu", font, 50);
    Text exitText("Exit", font, 50);

    // Helper function to draw rounded buttons
    auto drawRoundedButton = [&](Vector2f position, const string& text, Text& textObject, bool isHovered) {
        Color fillColor = isHovered ? hoverColor : buttonFillColor;

        // Base rectangle
        RectangleShape baseRect(Vector2f(buttonSize.x - 2 * cornerRadius, buttonSize.y));
        baseRect.setFillColor(fillColor);
        baseRect.setPosition(position.x + cornerRadius, position.y);

        // Circles for corners
        CircleShape topLeftCorner(cornerRadius);
        topLeftCorner.setFillColor(fillColor);
        topLeftCorner.setPosition(position);

        CircleShape topRightCorner(cornerRadius);
        topRightCorner.setFillColor(fillColor);
        topRightCorner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y);

        CircleShape bottomLeftCorner(cornerRadius);
        bottomLeftCorner.setFillColor(fillColor);
        bottomLeftCorner.setPosition(position.x, position.y + buttonSize.y - 2 * cornerRadius);

        CircleShape bottomRightCorner(cornerRadius);
        bottomRightCorner.setFillColor(fillColor);
        bottomRightCorner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y + buttonSize.y - 2 * cornerRadius);

        // Draw corners
        window.draw(topLeftCorner);
        window.draw(topRightCorner);
        window.draw(bottomLeftCorner);
        window.draw(bottomRightCorner);

        // Draw rectangle
        window.draw(baseRect);

        // Text
        textObject.setString(text);
        textObject.setFillColor(Color::White);
        FloatRect textBounds = textObject.getLocalBounds();
        textObject.setPosition(
            position.x + (buttonSize.x - textBounds.width) / 2,
            position.y + (buttonSize.y - textBounds.height) / 2 - 10
        );

        // Draw text
        window.draw(textObject);
        };

    // Helper function to check if mouse is hovering over a button
    auto isMouseOverButton = [&](FloatRect buttonBounds, Vector2i mousePos) -> bool {
        return buttonBounds.contains(Vector2f(mousePos));
        };

    // Pause menu logic
    while (window.isOpen()) {
        bool isResumeHovered = false, isSettingsHovered = false, isMainMenuHovered = false, isExitHovered = false;
        Event pauseEvent;

        while (window.pollEvent(pauseEvent)) {
            if (pauseEvent.type == Event::Closed) {
                window.close();  // Close the window
            }

            Vector2i mousePos = Mouse::getPosition(window); // Get mouse position in window coordinates

            // Check if mouse is over any button
            isResumeHovered = isMouseOverButton(FloatRect(780, 350, buttonSize.x, buttonSize.y), mousePos);
            isSettingsHovered = isMouseOverButton(FloatRect(780, 450, buttonSize.x, buttonSize.y), mousePos);
            isMainMenuHovered = isMouseOverButton(FloatRect(780, 550, buttonSize.x, buttonSize.y), mousePos);
            isExitHovered = isMouseOverButton(FloatRect(780, 650, buttonSize.x, buttonSize.y), mousePos);

            // Handle button clicks
            if (pauseEvent.type == Event::MouseButtonPressed && pauseEvent.mouseButton.button == Mouse::Left) {
                if (isResumeHovered) {
                    clickSound.play();
                    ispaused = false;
                    return ispaused; // Resume the game
                }
                if (isSettingsHovered) {
                    clickSound.play();
                    settingsWindow(window, font, selectedCarTexture, carSelected, ispaused, backgroundMusic,score,lives);
                    continue;
                }
                if (isMainMenuHovered) {
                    clickSound.play();
                    menuSection(window, font, selectedCarTexture, ispaused, carSelected, backgroundMusic,score,lives);
                    continue;
                }
                if (isExitHovered) {
                    clickSound.play();
                    window.close(); // Exit the game
                }
            }
        }

        window.clear(); // Clear the screen
        window.draw(pauseTitleBackgroundSprite);

        // Draw buttons with hover effects
        drawRoundedButton(Vector2f(780, 350), "Resume", resumeText, isResumeHovered);
        drawRoundedButton(Vector2f(780, 450), "Settings", settingsText, isSettingsHovered);
        drawRoundedButton(Vector2f(780, 550), "Main Menu", mainMenuText, isMainMenuHovered);
        drawRoundedButton(Vector2f(780, 650), "Exit", exitText, isExitHovered);

        window.display(); // Update the window
    }

    return 0; // Default return to indicate no action if the loop ends
}









int main() {
    // Create game window
    RenderWindow window(VideoMode(trackWidth, trackHeight), "Need For Speed: Ignition");
    bool carSelected = false;
    bool isPaused = false;
    int score = 0;
    int lives = 10;
    // showing the intro video firstly
    introVideo(window);


    // Load Font
    Font font;
    if (!font.loadFromFile(fontPath)) {
        cout << "Failed to load font from: " << fontPath << endl;
        return -1;
    }

    // Load Textures
    Texture fuelTexture, scoreTexture, livesTexture;
    if (!fuelTexture.loadFromFile(fuelPowerUpPath) ||
        !scoreTexture.loadFromFile(scorePowerUpPath) ||
        !livesTexture.loadFromFile(livesPowerUpPath)) {
        cout << "Error loading power-up textures!" << endl;
    }

    Texture selectedCarTexture;
    Music bgMusic;
    if (!bgMusic.openFromFile(backgroundMusicPath)) {
        cout << "Failed to load background music from: " << backgroundMusicPath << endl;
        return -1;
    }

    // after intro video calling main menu
    menuSection(window, font, selectedCarTexture, isPaused, carSelected, bgMusic, score, lives);

    // Load Background Music
    bgMusic.setLoop(true);
    bgMusic.play();

    // Load Collision Sound Effect
    SoundBuffer collisionBuffer;
    if (!collisionBuffer.loadFromFile(collisionSoundPath)) {
        cout << "Failed to load collision sound from: " << collisionSoundPath << endl;
        return -1;
    }
    Sound collisionSound(collisionBuffer);

    // HUD
    Text scoreText("Score: 0", font, 35);
    scoreText.setPosition(20, 20);
    scoreText.setFillColor(Color::Black);

    Text livesText("Lives: 3", font, 35);
    livesText.setPosition(20, 80);
    livesText.setFillColor(Color::Black);

    Text timeText("Time: 0.0s", font, 35);
    timeText.setPosition(1640, 76);
    timeText.setFillColor(Color::Black);

    // Array of level texts
    Text levels[10];
    for (int i = 0; i < 10; ++i) {
        levels[i].setFont(font);
        levels[i].setCharacterSize(50);
        levels[i].setPosition(30, 150);
        levels[i].setFillColor(Color::Black);
        levels[i].setString("Level: " + to_string(i + 1));
    }

    Sprite carSprite;
    carSprite.setTexture(selectedCarTexture);
    carSprite.setScale(2.0f, 2.0f);
    carSprite.setPosition(trackWidth / 2, trackHeight - 120);

    // Fuel Bar Background
    RectangleShape fuelBarBackground(Vector2f(30, 200));
    fuelBarBackground.setFillColor(Color::Black);
    fuelBarBackground.setPosition(1800, 100);

    // Fuel Bar
    RectangleShape fuelBar(Vector2f(30, 200));
    fuelBar.setFillColor(Color::Green);
    fuelBar.setPosition(1800, 100);

    // Use the selected map texture after selection
    Sprite backgroundTrackSprite;
    backgroundTrackSprite.setTexture(selectedMapTexture);
    backgroundTrackSprite.setScale(
        float(trackWidth) / selectedMapTexture.getSize().x,
        float(trackHeight) / selectedMapTexture.getSize().y
    );

    // image of obstacles
    vector<Sprite> obstacles;
    Texture obstacleTexture;
    if (!obstacleTexture.loadFromFile(obstacle1Path)) {
        cout << "Image not Found!";
        window.close();
    }
    // to generate the obstacles
    generateObstacles(obstacles, 15, Vector2f(trackWidth, trackHeight), obstacleTexture);

    // Power-Ups
    vector<Sprite> powerUps;
    int powerUpSize = 5; // Size of power-up
    Vector2f trackBounds(trackWidth, trackHeight);

    // Mini-map
    RenderTexture miniMapTexture;
    miniMapTexture.create(300, 300);
    Sprite miniMapSprite(miniMapTexture.getTexture());
    miniMapSprite.setPosition(1600, 700);

    CircleShape mapCar(3);
    mapCar.setFillColor(Color::Blue);

    CircleShape mapObstacle(3);
    mapObstacle.setFillColor(Color::Red);

    CircleShape mapPowerUp(3);
    mapPowerUp.setFillColor(Color::Green);

    // Game variables
 
    float carSpeed = 2.2f;
    int speedBoostDuration = 0;
    Clock clock;

    // Fuel Variables
    float maxFuel = 100.0f;
    float currentFuel = maxFuel;
    float fuelConsumptionRate = 0.05f;
    Clock gameClock;
    Clock updateClock;

    // Load previous playtime
    float previousPlayTime = 0.0f;
    float totalTime = 0.0f;

    // Game loop
    while (window.isOpen()) {
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            isPaused = true;
            bgMusic.pause();
            isPaused = showPauseMenu(window, font, isPaused, selectedCarTexture, carSelected, bgMusic,score,lives);
            continue;
        }

        if (!isPaused) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }
            // Handle player movement
            float dt = clock.restart().asSeconds();
            float elapsedTime = gameClock.getElapsedTime().asSeconds();
            // Update time text
            ostringstream oss;
            totalTime = elapsedTime + previousPlayTime;
            oss << "Time: " << fixed << setprecision(1) << totalTime << "s";
            timeText.setString(oss.str());

            // Moving the car
            if (Keyboard::isKeyPressed(Keyboard::Left) && carSprite.getPosition().x > 240) {
                carSprite.move(-carSpeed, 0);
            }
            if (Keyboard::isKeyPressed(Keyboard::Right) && carSprite.getPosition().x + carSprite.getGlobalBounds().width < 1680) {
                carSprite.move(carSpeed, 0);
            }
            if (Keyboard::isKeyPressed(Keyboard::Up) && carSprite.getPosition().y > 0) {
                carSprite.move(0, -carSpeed);
            }
            if (Keyboard::isKeyPressed(Keyboard::Down) && carSprite.getPosition().y + carSprite.getGlobalBounds().height < 1030) {
                carSprite.move(0, carSpeed);
            }

            // Control fuel
            if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Right)) {
                currentFuel -= fuelConsumptionRate;
                if (currentFuel <= 0) {
                    currentFuel = 0;
                    bgMusic.stop();
                    tryAgain(window, font, isPaused, carSelected, lives, score, currentFuel, carSprite, trackWidth, trackHeight, bgMusic, powerUps, totalTime, selectedCarTexture, previousPlayTime, gameClock);
                    bgMusic.play();
                }
            }
            checkForCheckpoint(score);

            // Calculate fuel bar height and adjust position
            float fuelBarHeight = (currentFuel / maxFuel) * 200;
            fuelBar.setSize(Vector2f(30, fuelBarHeight));
            fuelBar.setPosition(1800, 100 + (200 - fuelBarHeight));

            // Move obstacles only when the game is not paused
            if (!isPaused) {
                for (auto& obstacle : obstacles) {
                    obstacle.move(0, 0.5f * carSpeed);
                    if (obstacle.getPosition().y > trackHeight) {
                        float randomX = 300 + static_cast<float>(rand() % (1600 - 300 - static_cast<int>(obstacle.getGlobalBounds().width))); // Corrected type handling
                        obstacle.setPosition(randomX, 0);
                        score++;
                    }

                    // Check for collision with the car
                    if (carSprite.getGlobalBounds().intersects(obstacle.getGlobalBounds())) {
                        lives--;
                        collisionSound.play();
                        obstacle.setPosition(rand() % trackWidth, 0);
                        if (lives <= 0) {
                            bgMusic.stop();
                            savePlayTime(elapsedTime + previousPlayTime);
                            tryAgain(window, font, isPaused, carSelected, lives, score, currentFuel, carSprite, trackWidth, trackHeight, bgMusic, powerUps, totalTime, selectedCarTexture, previousPlayTime, gameClock);
                            bgMusic.play();
                        }
                    }
                }
            }

            // Manage power-ups (draw, generate, and handle collision)
            managePowerUps(window, carSprite, powerUps, fuelTexture, scoreTexture, livesTexture, currentFuel, score, speedBoostDuration, lives, trackBounds);

            // Update HUD
            scoreText.setString("Score: " + to_string(score));
            livesText.setString("Lives: " + to_string(lives));

            miniMapTexture.clear(Color(50, 50, 50, 150));

            // Draw obstacles on the mini-map
            for (const auto& obstacle : obstacles) {
                mapObstacle.setPosition(obstacle.getPosition().x / (trackWidth / 300.0f), obstacle.getPosition().y / (trackHeight / 300.0f));
                miniMapTexture.draw(mapObstacle);
            }

            // Draw power-ups on the mini-map
            for (const auto& powerUp : powerUps) {
                mapPowerUp.setPosition(powerUp.getPosition().x / (trackWidth / 300.0f), powerUp.getPosition().y / (trackHeight / 300.0f));
                miniMapTexture.draw(mapPowerUp);
            }

            // Draw the car on the mini-map
            mapCar.setPosition(carSprite.getPosition().x / (trackWidth / 300.0f), carSprite.getPosition().y / (trackHeight / 300.0f));
            miniMapTexture.draw(mapCar);

            miniMapTexture.display();

            // Render everything
            int levelIndex = score / 100;
            if (levelIndex > 9) {
                levelIndex = 9;
            }

            // Set the speed cap for the car
            carSpeed = (score >= 1000) ? 12.0f : 2.0f + (score / 100);

            window.clear();
            window.draw(backgroundTrackSprite);
            window.draw(carSprite);

            managePowerUps(window, carSprite, powerUps, fuelTexture, scoreTexture, livesTexture, currentFuel, score, speedBoostDuration, lives, trackBounds);
            window.draw(fuelBarBackground);
            window.draw(fuelBar);
            window.draw(scoreText);
            window.draw(livesText);
            window.draw(timeText);

            if (levelIndex >= 0 && levelIndex < 10) {
                window.draw(levels[levelIndex]);
            }

            for (const auto& obstacle : obstacles) {
                window.draw(obstacle);
            }
            window.draw(miniMapSprite);
            window.display();
        }
    }
    return 0;
}
