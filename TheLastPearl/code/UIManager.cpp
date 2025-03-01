// Name: Conor Ryan
// K-NUM: K00286377

#include "UIManager.h"
#include <iostream>

// Constructor for the UI manager. Initializes the UI manager.
UIManager::UIManager() {
}

// Initialize the HUD.
bool UIManager::init() {
    // Try to initialize the HUD.
    if (!hud.init()) {
        // If theres a problem return and print debug.
        std::cout << "Couldnt initialize HUD." << std::endl;
        return false;
    }

    // Create and set the properties of the new level button.
    sf::Vector2f buttonSize(100.f, 30.f);
    newLevelButton.setSize(buttonSize);
    newLevelButton.setFillColor(sf::Color(50, 50, 50, 200)); // Dark grey background.
    newLevelButton.setOutlineColor(sf::Color(255, 255, 255, 200)); // White border.
    newLevelButton.setOutlineThickness(2.f);

    newLevelButtonText.setFont(hud.getFont());
    newLevelButtonText.setString("New Level");
    newLevelButtonText.setCharacterSize(12);
    newLevelButtonText.setFillColor(sf::Color::White);

    return true; // Return true to indicate successful initialization.
}

// Process input events.
void UIManager::processEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Check if the tower menu is open.
    if (towerMenu.isOpen()) {
        // Check for mouse presses.
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            // Convert the mouse clicks position from pixel to world coordinates.
            sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

            // Create a rectangle which represents the tower menu.
            sf::FloatRect menuRect(towerMenu.getPosition().x, towerMenu.getPosition().y, towerMenu.getMenuWidth(), towerMenu.getMenuHeight());

            // If the click is outside the menu close it.
            if (!menuRect.contains(worldPos)) {
                towerMenu.close(soundManager);
                return;
            }

            // Pass the event to the tower menu for it to handle & return.
            towerMenu.handleInput(event, worldPos);
            return;
        } else if (event.type == sf::Event::KeyPressed) {
            // If a key is pressed pass it to the tower menu for it to handle & return.
            towerMenu.handleInput(event, sf::Vector2f());
            return;
        }
        return; // If no menu is open do nothing.
    }

    // Check if the generate new level button was clicked instead.
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        // Convert pixel to world coordinates.
        sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

        // Check if the generate new level button was clicked.
        if (newLevelButton.getGlobalBounds().contains(worldPos)) {
            newLevelButtonClicked = true; // Enable flag to begin generation.
        }
    }
}

// Updates HUD elements.
void UIManager::update(float deltaTime) {
    hud.update(deltaTime);
}

// Draws all UI elements ie. tower menu, new level button, and the HUD.
void UIManager::draw(sf::RenderWindow& window, int money, int lives, int wave, int kills, float timer,
    const std::vector<std::shared_ptr<Enemy>>& enemies,
    const std::vector<Tower>& towers)
{
    // Draw the menu if its open.
    if (towerMenu.isOpen()) {
        towerMenu.draw(window);
    }

    // Draw statistics for the current level on the HUD.
    hud.draw(window, money, lives, wave, kills, timer, enemies, towers);

    // Get to current view to position UI elements properly.
    sf::View view = window.getView();
    sf::Vector2f viewSize = view.getSize();
    float margin = 10.f; // Gap between the button and edge of the screen.

    // Position the new level button at the bottom right of the screen.
    newLevelButton.setPosition(view.getCenter().x + viewSize.x / 2 - newLevelButton.getSize().x - margin,
        view.getCenter().y + viewSize.y / 2 - newLevelButton.getSize().y - margin);

    // Adjust the texts postion to be centered within the button.
    sf::FloatRect textBounds = newLevelButtonText.getLocalBounds();
    newLevelButtonText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    newLevelButtonText.setPosition(newLevelButton.getPosition().x + newLevelButton.getSize().x / 2.0f, 
        newLevelButton.getPosition().y + newLevelButton.getSize().y / 2.0f);

    // Draw the button and text.
    window.draw(newLevelButton);
    window.draw(newLevelButtonText);
}

// Returns a reference to the TowerMenu object instance.
TowerMenu& UIManager::getTowerMenu() {
    return towerMenu;
}

// Checks if the tower menu is open currently.
bool UIManager::isTowerMenuOpen() const {
    return towerMenu.isOpen();
}

// Returns true if the new level button was clicked, and resets the flag.
bool UIManager::isNewLevelButtonClicked() {
    bool clicked = newLevelButtonClicked;
    newLevelButtonClicked = false;
    return clicked;
}