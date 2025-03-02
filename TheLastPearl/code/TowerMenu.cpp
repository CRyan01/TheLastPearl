// Name: Conor Ryan
// K-NUM: K00286377

#include "TowerMenu.h"
#include <sstream>
#include <iostream>

// Constructor for the tower menu. Initialzes the menu in a closed state with no selected option.
TowerMenu::TowerMenu() : openFlag(false), selectedOption(-1) {
    // Create a vector of options for the menu
    options.push_back({ "Pistol Tower", 150.0f });
    options.push_back({ "Rifle Tower", 150.0f });
    options.push_back({ "Cannon Tower", 150.0f });
    options.push_back({ "Carronade Tower", 150.0f });

    // Try to load the font from a file.
    if (!font.loadFromFile("fonts/OpenDyslexic3-Regular.ttf")) {
        // If theres a problem print debug.
        std::cout << "Couldnt load the tower menu font.\n";
    }

    // Loop through all options and create a text object for each.
    for (const auto& option : options) {
        sf::Text text; // Create the text object.
        text.setFont(font); // Set the font.
        std::stringstream ss; // Create a string stream.

        // Set properties.
        ss << option.name << " - $" << option.price;
        text.setString(ss.str());
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(18);

        // Save the text object to draw later.
        optionTexts.push_back(text);
    }
}

// Opens the tower menu at the specified coordinates.
void TowerMenu::open(const sf::Vector2i& gridPos, const sf::Vector2u& windowSize, SoundManager* soundManager) {
    const int tileSize = 64; // The size of an ingame tile in pixels.
    const int scoreboardMargin = 60; // Spacing for when the menu opens near the top of the screen.
    int menuHeight = getMenuHeight();  // Calculate menu height based on the number of available options.
    gridCoord = gridPos; // Store the grid coord of the tile the menu was opened from.

    // Position the menu to the top right of the selected tile.
    position.x = gridPos.x * tileSize + tileSize;
    position.y = gridPos.y * tileSize - menuHeight;

    // Check if the menu passes the right edge of the screen, if it does move it.
    if (gridPos.x > 25) {
        position.x = gridPos.x * tileSize - getMenuWidth();
    } else {
        position.x = gridPos.x * tileSize + tileSize;
    }
    position.y = gridPos.y * tileSize - menuHeight;

    // Check if the menu overlaps with the scoreboard. If it does move it below it.
    if (position.y < scoreboardMargin) {
        position.y = scoreboardMargin;
    }

    openFlag = true; // Mark the menu as open.
    selectedOption = -1; // Reset the currently selected option.

    // Check if the sound manager is valid & play a click sound.
    if (soundManager) {
        soundManager->playSound("click");
    }

    // Print debug
    std::cout << "Tower menu opened at gridPos (" << gridPos.x << ", " << gridPos.y
        << ") Top left position: (" << position.x << ", " << position.y << ")\n";
}

// Process input events for the tower menu.
void TowerMenu::handleInput(const sf::Event& event, const sf::Vector2f& worldPos) {
    // Check for a key press.
    if (event.type == sf::Event::KeyPressed) {
        // Assign an option based on which key was pressed.
        if (event.key.code == sf::Keyboard::Num1) {
            // Pistol tower.
            selectedOption = 0;
        } else if (event.key.code == sf::Keyboard::Num2) {
            // Rifle tower.
            selectedOption = 1;
        } else if (event.key.code == sf::Keyboard::Num3) {
            // Cannon tower.
            selectedOption = 2;
        } else if (event.key.code == sf::Keyboard::Num4) {
            // Carronade tower.
            selectedOption = 3;
        }  
    } 
    // Handle mouse pressed events.
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        int menuHeight = getMenuHeight(); // Calculate menu height based on the number of available options.

        // Create a rectangle the size of the tower menu to check clicks.
        sf::FloatRect menuRect(position.x, position.y, menuWidth, menuHeight);

        // Print debug.
        std::cout << "Mouse click at (" << worldPos.x << ", " << worldPos.y << ")\n";

        // Check if the click is within the menu.
        if (menuRect.contains(worldPos)) {
            // Store which option was clicked.
            float clickOffset = worldPos.y - position.y;
            int optionIndex = clickOffset / buttonHeight;
            
            // Check if a valid option was clicked.
            if (optionIndex >= 0 && optionIndex < options.size()) {
                // If it is valid, set the selected option to construct later.
                selectedOption = optionIndex;
                // Print debug.
                std::cout << "Valid tower option selected: " << optionIndex << "\n";
            } else {
                // Print debug if an invalid option was clicked.
                std::cout << "Invalid tower construction option...\n";
            }
        }
    }
}

// Returns the index of the selected option and resets selection.
int TowerMenu::getSelectedOption() {
    int selection = selectedOption; // Set the tower to construct.
    selectedOption = -1; // Reset the menus selected option.
    return selection; // Return the right tower to contruct.
}

void TowerMenu::draw(sf::RenderWindow& window) const {
    // Dont draw the menu if it isint open.
    if (!openFlag) {
        return;
    }

    // Calculate menu height based on the number of options.
    int menuHeight = getMenuHeight();

    // Create and draw the background for the menu.
    sf::RectangleShape bg(sf::Vector2f(menuWidth, menuHeight));
    bg.setFillColor(sf::Color(50, 50, 50, 200));
    bg.setPosition(position.x, position.y);
    window.draw(bg); // Draw the background.

    // Calculate button height based on the number of available options.
    int numOptions = options.size();
    float buttonHeight = menuHeight / numOptions;

    // Loop through all options and draw the selection box & text.
    for (int i = 0; i < numOptions; i++) {
        // Create a bounding box for each button.
        sf::FloatRect buttonArea(position.x, position.y + i * buttonHeight, menuWidth, buttonHeight);

        // Draw a white border around each button.
        sf::RectangleShape border(sf::Vector2f(buttonArea.width, buttonArea.height));
        border.setPosition(buttonArea.left, buttonArea.top);
        border.setFillColor(sf::Color::Transparent);
        border.setOutlineColor(sf::Color::White);
        border.setOutlineThickness(1.f);
        window.draw(border); // draw the border.

        // Position the text for each options.
        sf::Text text = optionTexts[i];
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
        text.setPosition(buttonArea.left + buttonArea.width / 2.f, buttonArea.top + buttonArea.height / 2.f);
        window.draw(text); // Draw the text.
    }
}

// Returns true if the tower menu is open.
bool TowerMenu::isOpen() const {
    return openFlag;
}

// Closes the tower menu and resets the selected option.
void TowerMenu::close(SoundManager* soundManager) {
    openFlag = false;
    selectedOption = -1;

    // Check if the sound manager is valid & play a click sound.
    if (soundManager) {
        soundManager->playSound("click");
    }
}

// Returns the top left position of the tower menu.
sf::Vector2i TowerMenu::getPosition() const {
    return position;
}

// Returns the grid coordiante of the tile the tower menu was opened from.
sf::Vector2i TowerMenu::getGridCoord() const {
    return gridCoord;
}
