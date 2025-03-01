// Name: Conor Ryan
// K-NUM: K00286377

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Map.h"
#include "Pathfinder.h"
#include "Enemy.h"
#include "Tower.h"
#include "Projectile.h"
#include "Player.h"
#include "UIManager.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "LevelManager.h"

// The game class handles initialization, the loop, updates, rendering, and cleanup.
class Game {
public:
    // Constructor - Constructs the game object.
    Game();

    // Initializes required members and resources.
    bool init();

    // The main loop.
    void run();

    // Cleans up resources and closes the game.
    void cleanup();

private:
    // Checks for collisions between projectiles and enemies.
    void checkCollisions();

    // Updates game logic.
    void update(float deltaTime);

    // Renders the game scene.
    void render();

    // Sets the view to a fixed virtual resolution.
    void adjustViewFixed();

    sf::RenderWindow window;                   // The games window
    bool running;                              // A flag which indicates if the game is running or not
    bool paused = false;                       // A flag to pause the game.

    Map gameMap;                               // The games map.
    Pathfinder pathfinder;                     // A pathfinder to create paths for enemies.
    std::vector<std::shared_ptr<Enemy>> enemies; // A list of all enemy pointers
    std::vector<Tower> towers;                 // A list of all towers
    std::vector<Projectile> projectiles;       // A list of all projectiles
    Player player;                             // The player.

    UIManager uiManager;                       // Manages UI elements.
    SoundManager soundManager;                 // Handles game sounds.
    InputManager inputManager;                 // Processes input events.
    LevelManager levelManager;                 // Manages enemy waves and levels.

    sf::Clock clock;                           // Keeps track of elapsed time.
};
