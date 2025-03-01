// Name: Conor Ryan
// K-NUM: K00286377

#pragma once

#include <SFML/Graphics.hpp>
#include "TowerMenu.h"
#include "HUD.h"

// The UI handles all user interface elements ie. HUD, tower menu, new level button.
class UIManager {
public:
    // Default constructor for the HUD.
    UIManager();

    // Initializes UI components.
    bool init();

    // Processes an input event, the window parameter is used for coordinate conversion.
    void processEvent(const sf::Event& event, sf::RenderWindow& window);

    // Updates UI elements using time.
    void update(float deltaTime);

    // Draws the UI, including healthbars.
    void draw(sf::RenderWindow& window, int money, int lives, int wave, int kills, float timer,
        const std::vector<std::shared_ptr<class Enemy>>& enemies,
        const std::vector<class Tower>& towers);

    // Returns a reference to the tower menu.
    TowerMenu& getTowerMenu();

    // Returns true if the tower menu is open.
    bool isTowerMenuOpen() const;

    // Returns true if the new level button was clicked.
    bool isNewLevelButtonClicked();

private:
    TowerMenu towerMenu;  // Menu for selecting tower options.
    HUD hud;              // Heads-up display with global counters and indicators.

    sf::RectangleShape newLevelButton; // Button background.
    sf::Text newLevelButtonText;         // Button text.
    bool newLevelButtonClicked = false;  // Flag set when button is clicked.
};
