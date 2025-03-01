// Name: Conor Ryan
// K-NUM: K00286377

#pragma once

// The player class manages the players money and life count.
class Player {
public:
    // Constructor - creates a player with deafult money and lives.
    Player();

    // Returns true if the player can afford the specified amount.
    bool canAfford(float amount) const;

    // Takes the specified amount from the player.
    void spend(float amount);

    // Gives the specified amount to the player.
    void earn(float amount);

    // Returns how much money the player has.
    int getMoney() const;

    // Returns how many lives the player has left.
    int getLives() const;

    // Reduces the players life by one.
    void loseLife();

    // Restores the players lives to the default value.
    void resetLives();

private:
    int money;  // How much money the player has.
    int lives;  // How many lives the player has.
};
