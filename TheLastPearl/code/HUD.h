// Name: Conor Ryan
// K-NUM: K00286377

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>
#include "Enemy.h"
#include "Tower.h"

// The HUD class handles the display of ingame counters.
class HUD {
public:
    // A default constructor for the HUD.
    HUD();

    // Initializes the HUD's resources.
    bool init();

    // Update function for the HUD.
    void update(float deltaTime);
    
    // Draws the HUD in the SFML window.
    void draw(sf::RenderWindow& window, int money, int lives, int wave, int kills, float timer,
        const std::vector<std::shared_ptr<Enemy>>& enemies,
        const std::vector<Tower>& towers);

    // Returns the font used for the HUD.
    const sf::Font& getFont() const { return font; }

private:
    sf::Font font; // The font used to display the text.

    // Converts an enemy type to its string version for nameplates.
    std::string enemyTypeToString(EnemyType type) const;

    // Converts an enemy type to its string version for nameplates.
    std::string towerTypeToString(TowerType type) const;
};
