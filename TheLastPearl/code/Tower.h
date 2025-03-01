// Name: Conor Ryan
// K-NUM: K00286377

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Enemy.h"
#include "Projectile.h"
#include "TowerType.h"
#include "SoundManager.h"

// The tower class represents a tower.
class Tower {
public:
    // Constructs a tower with specified attributes.
    Tower(const sf::Vector2f& pos, TowerType type,
        float damagePerShot = 25.0f, int burstCount = 1,
        float fireRate = 1.0f, float cooldown = 0.0f);

    // Updates the tower by checking for enemies in range and firing projectiles.
    void update(float deltaTime, const std::vector<std::shared_ptr<Enemy>>& enemies,
        std::vector<Projectile>& projectiles, SoundManager* soundManager);

    // Draws the tower using a simple rectangle shape.
    void draw(sf::RenderWindow& window) const;

    // Returns the tower's price.
    float getPrice() const;

    // Computes the DPS of the tower.
    float calculateDPS() const;

    // Returns the tower type.
    TowerType getType() const { return type; }

    // Returns the tower's position.
    sf::Vector2f getPosition() const { return position; }

private:
    sf::Vector2f position; // The towers position.
    TowerType type; // The type of tower.

    float damagePerShot; // How much damage the tower does per shot.
    int burstCount; // How many shots the tower first per burst.
    float fireRate; // The towers rate of fire.
    float cooldown; // The cooldown between bursts.
    float timeSinceLastShot = 0.0f; // Time elapsed since the tower last shot.
    float attackRange = 100.0f; // The towers attack range.

    sf::RectangleShape visual;  // A visual representation of the tower.

    void setupVisual(); // Adjusts the towers visual based on its type.
};
