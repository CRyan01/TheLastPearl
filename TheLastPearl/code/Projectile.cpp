// Name: Conor Ryan
// K-NUM: K00286377

#include "Projectile.h"
#include <cmath>

// Parameterized constructor for a projectile, initializes a projectile with specific attributes.
Projectile::Projectile(const sf::Vector2f& pos, const sf::Vector2f& direction,
    float speed, float damage, float radius, sf::Color color,
    float maxRange)
    : position(pos), startPosition(pos), direction(direction), speed(speed), damage(damage), maxRange(maxRange)
{
    // Set the projectiles properties.
    shape.setRadius(radius);
    shape.setFillColor(color);
    shape.setOrigin(radius, radius);
    shape.setPosition(position);
}

// Update the projectiles movement based on elapsed time.
void Projectile::update(float deltaTime) {
    position += direction * speed * deltaTime;
    shape.setPosition(position);

    // Calculate how far the projectile has traveled.
    float dx = position.x - startPosition.x;
    float dy = position.y - startPosition.y;
    float traveled = std::sqrt(dx * dx + dy * dy);

    // If the projectile passes its max range deactivate it, so that it can be cleaned up.
    if (traveled > maxRange) {
        active = false;
    }
}

// Draws the projectile.
void Projectile::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

// Returns true if the projectile is still active.
bool Projectile::isActive() const {
    return active;
}