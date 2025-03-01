// Name: Conor Ryan
// K-NUM: K00286377

#pragma once

#include <SFML/Graphics.hpp>

// The Projectile class represents a projectile shot by a tower.
class Projectile {
public:
    // Constructor - creates a projectile with specifed attributes.
    Projectile(const sf::Vector2f& pos, const sf::Vector2f& direction,
        float speed, float damage, float radius = 5.0f,
        sf::Color color = sf::Color::Yellow, float maxRange = 500.0f);

    // Updates the projectiles poistion based on time elapsed.
    void update(float deltaTime);

    // Draws the projectile in the SFML view.
    void draw(sf::RenderWindow& window) const;

    // Returns true if the projectile is active.
    bool isActive() const;

    // Returns the projectiles bounding box for collision checks.
    sf::FloatRect getGlobalBounds() const { return shape.getGlobalBounds(); }

    // Returns the projectiles damage.
    float getDamage() const { return damage; }

    // Marks the projectile as inactive.
    void markInactive() { active = false; }

private:
    sf::Vector2f position;       // The projectiles current position.
    sf::Vector2f startPosition;  // The projectiles starting position.
    sf::Vector2f direction;      // The projectiles direction of movement.
    float speed;                 // The projectiles speed in pixels.
    float damage;                // The amount of damage the projectile deals on impact.
    bool active = true;          // A flag which indicates if the projectile is still active.
    float maxRange;              // The maximum distance a projectile can travel.

    sf::CircleShape shape;       // A visual representation of the projectile.
};
