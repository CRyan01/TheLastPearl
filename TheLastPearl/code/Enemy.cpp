// Name: Conor Ryan
// K-NUM: K00286377

#include "Enemy.h"
#include <cmath>

// Parameterized constructor for the enemy class. Initalizes an enemy with specified attributes.
Enemy::Enemy(const sf::Vector2f& startPos, const std::vector<sf::Vector2i>& path, EnemyType type, float health)
    : position(startPos), path(path), health(health), maxHealth(health), type(type)
{
    shape.setRadius(10.0f); // Set the enemys shape to a circle.

    // Set the enemies color based on its type.
    switch (type) {
    case EnemyType::Marauder:
        shape.setFillColor(sf::Color::Black);
        break;
    case EnemyType::Privateer:
        shape.setFillColor(sf::Color::Magenta);
        break;
    case EnemyType::Corsair:
        shape.setFillColor(sf::Color::White);
        break;
    case EnemyType::Captain:
        shape.setFillColor(sf::Color::Red);
        break;
    }
    // Center the shapes origin and set its initial position.
    shape.setOrigin(10.0f, 10.0f);
    shape.setPosition(position);
}

// Updates the enemies position. Moves them along a predefined path constructed by the pathfinder.
void Enemy::update(float deltaTime, int tileSize) {
    if (currentWaypoint >= path.size()) { // Return if the enemy has already reached its destination.
        return;
    }

    // Calculate the center position of the tile the current waypoint points to.
    sf::Vector2f targetPos(path[currentWaypoint].x * tileSize + tileSize / 2.0f,
        path[currentWaypoint].y * tileSize + tileSize / 2.0f);
    // Calculate the direction vector that leads towards the target waypoint.
    sf::Vector2f direction = targetPos - position;
    // Calculate the distance to the target waypoint.
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Normailze the direction vector to make sure movement stays consistent.
    if (distance != 0) {
        direction /= distance;
    }

    // Move the enemy towards the waypoint based on speed and time elapsed.
    position += direction * speed * deltaTime;
    shape.setPosition(position);

    // If the enemy is close to the current waypoint, cycle to the next one.
    if (distance < 5.0f && currentWaypoint < path.size()) {
        currentWaypoint++;
    }
}

// Draws the enemies shape in the SFML view.
void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

// Checks if the enemy reached the end of its path.
bool Enemy::reachedEnd() const {
    return currentWaypoint >= path.size();
}

// Checks if the enemy is still alive.
bool Enemy::isAlive() const {
    return health > 0;
}

// Reduce the enemies health by a specified amount.
void Enemy::takeDamage(float damage) {
    health -= damage;
}