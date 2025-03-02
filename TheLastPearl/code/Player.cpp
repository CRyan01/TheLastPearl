// Name: Conor Ryan
// K-NUM: K00286377

#include "Player.h"

// Constructor for the player class, initialze the starting amount of lives and money available.
Player::Player() : money(10000), lives(20) {

}

// Checks if the player can afford the specified amount
bool Player::canAfford(float amount) const {
    return money >= (amount);
}

// Remove a specified amount from the players money pool.
void Player::spend(float amount) {
    money -= (amount);
}

// Adds a specified amount to the players money pool.
void Player::earn(float amount) {
    money += (amount);
}

// Returns the current amount of money available to the player.
int Player::getMoney() const {
    return money;
}

// Returns the current amount of lives the player has available.
int Player::getLives() const {
    return lives;
}

// Remove a life from the player if there are any left.
void Player::loseLife() {
    if (lives > 0)
        lives--;
}

// Resests the players lives back to 20.
void Player::resetLives() {
    lives = 20;
}

// Resests the players money back to 10000.
void Player::resetMoney() {
    money = 10000;
}