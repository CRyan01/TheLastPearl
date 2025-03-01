// Name: Conor Ryan
// K-NUM: K00286377

#include "HUD.h"

// Constructor - Initializes the HUD.
HUD::HUD() {
}

// Initialize the required font.
bool HUD::init() {
    // Try to load the font.
    if (!font.loadFromFile("fonts/OpenDyslexic3-Regular.ttf")) {
        // If theres a problem return false.
        return false;
    }
    return true; // Otherwise return true.
}

// Updates the HUD.
void HUD::update(float deltaTime) {
    
}

// Converts an EnemyType enum to its correct string version.
std::string HUD::enemyTypeToString(EnemyType type) const {
    // Return the appropriate string.
    switch (type) {
        case EnemyType::Marauder:
            return "Marauder";
        case EnemyType::Privateer:
            return "Privateer";
            break;
        case EnemyType::Corsair:
            return "Corsair";
            break;
        case EnemyType::Captain:
            return "Captain";
            break;
        default:
            return "Enemy";
    }
}

// Converts an TowerType enum to its correct string version.
std::string HUD::towerTypeToString(TowerType type) const {
    // Return the appropriate string.
    switch (type) {
    case TowerType::Pistol:
        return "Pistol";
    case TowerType::Rifle:
        return "Rifle";
        break;
    case TowerType::Cannon:
        return "Cannon";
        break;
    case TowerType::Carronade:
        return "Carronade";
        break;
    default:
        return "Tower";
    }
}

// Draws the HUD elements i.e scoreboard, enemy health bars and name plates, tower nameplates.
void HUD::draw(sf::RenderWindow& window, int money, int lives, int wave, int kills, float timer,
    const std::vector<std::shared_ptr<Enemy>>& enemies,
    const std::vector<Tower>& towers)
{   
    // Create a text object for the HUD and set its properties.
    sf::Text hudText;
    hudText.setFont(font);
    hudText.setCharacterSize(14);
    hudText.setFillColor(sf::Color::White);

    // Convert the time elapsed in level to minutes and seconds.
    int totalSeconds = timer;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    // Create the HUD string stream.
    std::stringstream ss;
    ss << "Money: " << money
        << "   Lives: " << lives
        << "   Wave: " << wave
        << "   Kills: " << kills
        << "   Time: " << minutes << ":";
    if (seconds < 10) {
        ss << "0";  // Add a 0 before seconds if it is less than 10.
    }
    ss << seconds;

    // Set the HUD text.
    hudText.setString(ss.str());

    // Center the HUDs text at the top of the screen.
    sf::View view = window.getView();
    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f viewCenter = view.getCenter();
    sf::FloatRect textBounds = hudText.getLocalBounds();
    // Center the texts origin
    hudText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    // Set the texts position
    hudText.setPosition(viewCenter.x, viewCenter.y - viewSize.y / 2 + 20.f);

    // Draw a background recntangle behind the HUD to make it more visible.
    sf::FloatRect textGlobalBounds = hudText.getGlobalBounds();
    float padding = 10.0f; // Add padding
    sf::RectangleShape background(sf::Vector2f(textGlobalBounds.width + 2 * padding, textGlobalBounds.height + 2 * padding));
    background.setFillColor(sf::Color(50, 50, 50, 200));  // Dark gray, transparent
    background.setOutlineThickness(2.f);                  // Border thickness
    background.setOutlineColor(sf::Color(255, 255, 255, 200)); // Border color = transparent white.
    // Position the background so that it covers the text.
    background.setPosition(textGlobalBounds.left - padding, textGlobalBounds.top - padding);

    window.draw(background); // Draw the background.
    window.draw(hudText); // Draw the HUD text on top of it.

    // Loop through all enemies to draw healthbars and nameplates.
    for (const auto& enemy : enemies) {
        // Skip any dead enemies.
        if (!enemy->isAlive()) {
            continue;
        }

        // Get the enemies position.
        sf::Vector2f pos = enemy->getShape().getPosition();

        // Set the size of the healthbar.
        float barWidth = 30.f, barHeight = 4.f;

        // Calculate the ratio of current health to max health.
        float healthRatio = enemy->getHealth() / enemy->getMaxHealth();
        float offsetY = 25.f; // Apply a verical offset to move the bar above the enemy.

        // Create and configure the red bar.
        sf::RectangleShape bgBar(sf::Vector2f(barWidth, barHeight));
        bgBar.setFillColor(sf::Color::Red);
        bgBar.setOrigin(barWidth / 2, barHeight / 2);
        bgBar.setPosition(pos.x, pos.y - offsetY);

        // Create and configure the green bar.
        sf::RectangleShape fgBar(sf::Vector2f(barWidth * healthRatio, barHeight));
        fgBar.setFillColor(sf::Color::Green);
        fgBar.setOrigin(barWidth / 2, barHeight / 2);
        fgBar.setPosition(pos.x, pos.y - offsetY);

        window.draw(bgBar); // Draw the red background bar.
        window.draw(fgBar); // Draw the green foreground bar.

        // Create a text object to add a nameplate to the enemy.
        sf::Text enemyName;
        enemyName.setFont(font);
        enemyName.setString(enemyTypeToString(enemy->getType()));
        enemyName.setCharacterSize(10);
        enemyName.setFillColor(sf::Color::White);

        // Center the nameplate.
        sf::FloatRect enemyTextBounds = enemyName.getLocalBounds();
        enemyName.setOrigin(enemyTextBounds.left + enemyTextBounds.width / 2, enemyTextBounds.top + enemyTextBounds.height / 2);
        // Set the position for the nameplate using the vertical offset.
        enemyName.setPosition(pos.x, pos.y - offsetY - 10.f);

        window.draw(enemyName); // Draw the nameplate.
    }

    // Loop through all towers and draw a nameplate above them.
    for (const auto& tower : towers) {
        sf::Vector2f position = tower.getPosition(); // Retrieve the towers position.
        sf::Text towerLabel; // Create a text object for the label.
        towerLabel.setFont(font); // Set the labels font.

        // Set the labels properties and add a number as a prefix to the label - expand further.
        towerLabel.setString("1 " + towerTypeToString(tower.getType()));
        towerLabel.setCharacterSize(10);
        towerLabel.setFillColor(sf::Color::White);

        // Center the towers label.
        sf::FloatRect towerTextBounds = towerLabel.getLocalBounds();
        towerLabel.setOrigin(towerTextBounds.left + towerTextBounds.width / 2,
            towerTextBounds.top + towerTextBounds.height / 2);
        // Position the label above the tower.
        towerLabel.setPosition(position.x, position.y - 30.f);

        // Draw the label.
        window.draw(towerLabel);
    }
}