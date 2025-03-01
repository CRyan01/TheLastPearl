// Name: Conor Ryan
// K-NUM: K00286377

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "EnemyType.h"

// The Enemy class represents an enemy unit.
class Enemy {
public:
    // Creates an enemy with a spcified start position, path (a list of coordinates), and enemy type. Initial health is set to 100.
    Enemy(const sf::Vector2f& startPos, const std::vector<sf::Vector2i>& path, EnemyType type, float health = 100.0f);
    
    // Updates the enemies state and position, pass in tileSize to convert grid coords.
    void update(float deltaTime, int tileSize);

    // Draws the enemy on the SFML view.
    void draw(sf::RenderWindow& window) const;

    // Returns true if an enemy reached its target.
    bool reachedEnd() const;

    // Returns true if an enemy is alive.
    bool isAlive() const;

    // Reduces the enemies health by a specificed amount.
    void takeDamage(float damage);

    // Returns a reference to the enemies shape.
    const sf::CircleShape& getShape() const { return shape; }

    // Returns the enemies type.
    EnemyType getType() const { return type; }

    // Returns the current health of an enemy.
    float getHealth() const { return health; }

    // Returns the max health of an enemy.
    float getMaxHealth() const { return maxHealth; }

private:
    sf::Vector2f position;              // Current position on the map.
    std::vector<sf::Vector2i> path;     // The grid path the enemy will follow.
    size_t currentWaypoint = 0;         // The index of the next waypoint.
    float speed = 50.0f;                // Movement speed.
    float health;                       // Current health.
    float maxHealth;                    // Max health.
    EnemyType type;                     // The enemies type.
    sf::CircleShape shape;              // A visual representation of the enemy.
    sf::Texture enemyTexture;           // A texture for the enemies sprite.
    sf::Sprite enemySprite;             // A sprite to represent the enemy.
};
