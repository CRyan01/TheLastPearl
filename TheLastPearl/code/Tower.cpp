// Name: Conor Ryan
// K-NUM: K00286377

#include "Tower.h"
#include "CombatUtilities.h"
#include <cmath>
#include <iostream>

sf::Texture Tower::towerTexture; // load only once.

// A parameterized constructor to initialize the tower class with specified attributes.
Tower::Tower(const sf::Vector2f& pos, TowerType type,
    float damagePerShot, int burstCount, float fireRate, float cooldown)
    : position(pos), type(type), damagePerShot(damagePerShot),
    burstCount(burstCount), fireRate(fireRate), cooldown(cooldown)
{
    setupVisual();
    visual.setPosition(position);
}

// Sets up the visual apperance of a tower based on its type.
void Tower::setupVisual() {
    visual.setSize(sf::Vector2f(40, 40)); // Set a size of 40x40.

    // Try to load a texture for the tower.
    if (!towerTexture.loadFromFile("graphics/tower.png")) {
        std::cout << "Couldnt load the tower texture.\n";
    }
    // Set the sprites texture.
    towerSprite.setTexture(towerTexture);

    // Center the sprites origin.
    sf::FloatRect spriteBounds = towerSprite.getLocalBounds();
    towerSprite.setOrigin(spriteBounds.width / 2.0f, spriteBounds.height / 2.0f);

    // Set the sprites scale and position.
    towerSprite.setScale(0.8f, 0.8f);
    towerSprite.setPosition(position);

    // Set a color based on the towers type.
    switch (type) {
    case TowerType::Pistol:
        visual.setFillColor(sf::Color::Cyan);
        break;
    case TowerType::Rifle:
        visual.setFillColor(sf::Color::Green);
        break;
    case TowerType::Cannon:
        visual.setFillColor(sf::Color::Blue);
        break;
    case TowerType::Carronade:
        visual.setFillColor(sf::Color::Yellow);
        break;
    }
    // Set the origin of the tower to the center.
    sf::FloatRect bounds = visual.getLocalBounds();
    visual.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
}

// Updates the tower, checks for enemies and fires projectiles when they enter the towers range.
void Tower::update(float deltaTime, const std::vector<std::shared_ptr<Enemy>>& enemies,
    std::vector<Projectile>& projectiles, SoundManager* soundManager) {
    // Store the amount of time passed since the tower last fired.
    timeSinceLastShot += deltaTime;

    // Loop through all enemies.
    for (const auto& enemy : enemies) {
        // Check if the currently selected enemy is alive, to avoid targeting "dead" ones.
        if (enemy->isAlive()) {
            // Get the vector difference between the enemies position and the towers position.
            sf::Vector2f difference = enemy->getShape().getPosition() - position;

            // Calculate the distance to the enemy using Pythagoras theorem.
            float distance = std::sqrt(difference.x * difference.x + difference.y * difference.y);

            // Check if the enemy is within range, and if enough time passed since the tower last fired.
            if (distance <= attackRange && timeSinceLastShot >= (cooldown + fireRate * burstCount)) {

                // Store the difference between the enemies and towers position.
                sf::Vector2f direction = difference;

                // Calculate the length of the vector using Pythagoras theorem.
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (length != 0) {
                    direction /= length;
                }
                // Calculate the damage multiplier for the shots damage based on the type of tower shooting, and type of enemy being shot at.
                float multiplier = calculateDamageMultiplier(this->type, enemy->getType());
                
                // Calculate the final damage for shot based on the towers burst count (num shots fired), and damage multiplier.
                float finalDamage = damagePerShot * burstCount * multiplier;

                // Create a new projectile with attributes based on which tower it was shot from.
                switch (type) {
                    case TowerType::Pistol: {
                        // Pistol tower.
                        projectiles.emplace_back(position, direction, 300.0f, finalDamage, 3.0f, sf::Color(70, 70, 70), 150.0f);

                        // Check if the sound manager is valid and play a shoot sound.
                        if (soundManager) {
                            soundManager->playSound("pistol");
                        }
                        break;
                    }
                    case TowerType::Rifle: {
                        // Rifle tower.
                        projectiles.emplace_back(position, direction, 350.0f, finalDamage, 5.0f, sf::Color(70, 70, 70), 250.0f);

                        // Check if the sound manager is valid and play a shoot sound.
                        if (soundManager) {
                            soundManager->playSound("rifle");
                        }
                        break;
                    }
                    case TowerType::Cannon: {
                        // Cannon tower.
                        projectiles.emplace_back(position, direction, 150.0f, finalDamage, 10.0f, sf::Color(70, 70, 70), 500.0f);

                        // Check if the sound manager is valid and play a shoot sound.
                        if (soundManager) {
                            soundManager->playSound("cannon");
                        }
                        break;
                    }
                    case TowerType::Carronade: {
                        // Carronade tower
                        const int numProjectiles = 5; // The number of projectiles fired in one shot.
                        float spreadAngle = 15.0f; // The spread angle of the shot, in degrees.
                        float spreadRad = spreadAngle * 3.14159f / 180.0f; // Convert degrees to radians.

                        // Loop through each projectile in the shot.
                        for (int i = 0; i < numProjectiles; i++) {
                            // Calculate an angle offset for each projectile.
                            float angleOffset = ((float)i - (numProjectiles - 1) / 2.0f) * spreadRad;
                            // Calculate the base angle.
                            float baseAngle = std::atan2(direction.y, direction.x);
                            // Calculate a new angle for each projectile.
                            float newAngle = baseAngle + angleOffset;
                            // Convert the new angle back to a directional vector.
                            sf::Vector2f newDirection(std::cos(newAngle), std::sin(newAngle));
                            // Create a projectile with the new spread direction.
                            projectiles.emplace_back(position, newDirection, 180.0f, finalDamage, 7.0f, sf::Color(70, 70, 70), 150.0f);

                            // Check if the sound manager is valid and play a shoot sound.
                            if (soundManager) {
                                soundManager->playSound("carronade");
                            }
                        }
                        break;
                    }
                }
                timeSinceLastShot = 0.0f; // Reset the time since the tower last fired.
                break; // Dont check further enemies once the tower has fired.
            }
        }
    }
}

// Draws the tower in the specified window.
void Tower::draw(sf::RenderWindow& window) const {
    // window.draw(visual);
    window.draw(towerSprite);
}

// A function to calculate the damage of a towers shot.
float Tower::calculateDPS() const {
    return (damagePerShot * burstCount) / (cooldown + (fireRate * burstCount));
}