// Name: Conor Ryan
// K-NUM: K00286377

#include "Game.h"
#include "LevelManager.h"
#include <iostream>

// Constructs the game object, create a window, initializes various managers, and generates the initial level.
Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "The Last Pearl", sf::Style::Fullscreen), // Set up the window.
    running(true), // Flag to indicate that the game is running and not paused.
    gameMap(32, 18, 300, 1, 6), // Create a custom game map: sizeX, sizeY, clusterCount, minClusterSize, maxClusterSize.
    uiManager(), // Init the UI manager.
    inputManager(uiManager), // Init the input manager and pass it the UI manager.
    levelManager(gameMap) // Init the levelmanger and pass it the Map object.
{
    gameMap.generateRandom(); // Generate a new random level at the start. Could provide an option to read from file instead.
}

// Initializes the game,
bool Game::init() {
    uiManager.getTowerMenu().close(&soundManager);
    uiManager.setSoundManager(&soundManager); // Pass a reference to the sound manager to the ui manager.

    // Load the necessary sounds.
    soundManager.loadSound("pistol", "sounds/pistol.ogg");
    soundManager.loadSound("rifle", "sounds/rifle.ogg");
    soundManager.loadSound("cannon", "sounds/cannon.ogg");
    soundManager.loadSound("carronade", "sounds/carronade.ogg");
    soundManager.loadSound("click", "sounds/click.ogg");
    soundManager.loadSound("hit", "sounds/hit.ogg");
    soundManager.loadSound("death", "sounds/death.ogg");
    soundManager.loadSound("construction", "sounds/construction.ogg");
    soundManager.playBackgroundMusic("sounds/background.ogg");

    // Setup a fixed resolution to make the view consistent.
    adjustViewFixed();

    // Initialize the UI manager, return if there is a problem.
    if (!uiManager.init()) {
        return false;
    }

    // Set up an input callback to handle window input events.
    inputManager.setGameEventHandler([this](const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                // If 'Escape' is pressed close the game.
                window.close();
            } else if (event.key.code == sf::Keyboard::P) {
                // If 'P' is pressed pause the game.
                paused = !paused;
                
                // Print debug.
                if (paused) {
                    std::cout << "Paused the game.\n";
                } else if (!paused) {
                    std::cout << "Unpaused the game.\n";
                }
            }
        }

        // Handle mouse pressed events.
        if (event.type == sf::Event::MouseButtonPressed) {
            // Check if the tower menu is already open.
            if (uiManager.getTowerMenu().isOpen()) {
                return; // If it is return to avoid opening a new one.
            }

            // Convert the mouse click to pixel coordinates.
            sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            const int tileSize = 64;
            int gridX = (worldPos.x) / tileSize;
            int gridY = (worldPos.y) / tileSize;

            // If the tile clicked is a wall tile open the tower menu.
            if (gameMap.getTile(gridX, gridY).type == TileType::Wall) {
                // Close any previously open menus.
                uiManager.getTowerMenu().close(&soundManager);
                // Open a new menu for the clicked wall tile.
                uiManager.getTowerMenu().open({ gridX, gridY }, window.getSize(), &soundManager);
            } else {
                // If a ground tile was clicked close the menu.
                uiManager.getTowerMenu().close(&soundManager);
            }
        }
        });
    return true;
}

// Sets the view to a fixed resolution based on map size.
void Game::adjustViewFixed() {
    // Calculate a resolution based on map size.
    const int tileSize = 64;
    int width = gameMap.getWidth() * tileSize;
    int height = gameMap.getHeight() * tileSize;

    // Create the new view and set it.
    sf::View view(sf::FloatRect(0, 0, width, height));
    window.setView(view);
}

// The main game loop.
void Game::run() {
    // Keep running until the running flag is false.
    while (running && window.isOpen()) {
        // Store the time elapsed.
        float deltaTime = clock.restart().asSeconds();
        update(deltaTime); // Update the game.
        render(); // Draw the game objects.
    }
}

// Check for collisions between enemies and projectiles,
// handle any hits, and clean up inactive enemies and projectiles.
void Game::checkCollisions() {
    // Loop through all projectiles
    for (auto& projectile : projectiles) {
        // Check if the currently selected projectile is active.
        if (!projectile.isActive()) {
            continue; // If it is proceed. Otherwise skip.
        }

        // Store the global bounds of the projectile.
        sf::FloatRect projectileBounds = projectile.getGlobalBounds();

        // Loop through all enemies.
        for (auto& enemy : enemies) {
            // Check if the currently selected enemy is alive.
            if (!enemy->isAlive()) {
                continue; // If it is continue. Otherwise skip it.
            }

            // Store the global bounds of the enemies shape.
            sf::FloatRect enemyBounds = enemy->getShape().getGlobalBounds();

            // Check for collisions between the selected enemy and projectile.
            if (projectileBounds.intersects(enemyBounds)) {
                // If there is a collision, apply damage to the enemy.
                enemy->takeDamage(projectile.getDamage());
                soundManager.playSound("hit"); // Play an impact sound.
                projectile.markInactive(); // Mark the projectile as inactive.
                break; // Make sure only one enemy is hit per projectile.
            }
        }
    }

    // Cleanup any projectiles that are inactive.
    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        // Check if the currently selected projectile is inactive.
        if (!it->isActive()) {
            it = projectiles.erase(it); // If it is delete it to free resources.
        } else {
            ++it; // If it isint proceed to the next projectile in the vector.
        }
    }

    // Cleanup any enemies that are inactive.
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        // Check if the currently selected enemy is inactive.
        if (!(*it)->isAlive()) {
            it = enemies.erase(it); // If it is delete it to free resources.
        } else {
            ++it; // If it isint proceed to the next enemy in the vector.
        }
    }
}

// Updates all game objects i.e towers, enemies, and the UI.
void Game::update(float deltaTime) {
    // Setup the input manager to handle input events in the window.
    inputManager.processInput(window);

    // If the game is paused only update the UI manager.
    if (paused) {
        uiManager.update(deltaTime);
        return;
    }

    // Check if the tower menu is open and handle tower construction.
    if (uiManager.getTowerMenu().isOpen()) {
        // Store the option that was clicked in the tower menu.
        int selectedOption = uiManager.getTowerMenu().getSelectedOption(); 

        // Check that something valid was clicked. If it is proceed with tower construction.
        if (selectedOption >= 0) {
            TowerType selectedType; // Create a enum type to apply to the tower.
            float price; // Store a price for the towers.

            // Check which type of tower was clicked and set its type enum, and price.
            switch (selectedOption) {
                case 0:
                    // Pistol tower.
                    selectedType = TowerType::Pistol;
                    price = 300.0f;
                    break;
                case 1:
                    // Rifle tower.
                    selectedType = TowerType::Rifle;
                    price = 500.0f;
                case 2:
                    // Cannon tower.
                    selectedType = TowerType::Cannon;
                    price = 1000.0f;
                    break;
                case 3:
                    // Carronade tower.
                    selectedType = TowerType::Carronade;
                    price = 3000.0f;
                default:
                    // Print debug if theres an issue.
                    std::cout << "Invalid selected option.\n";
            }

            // Check if the player can afford to buy the selected tower before building it.
            if (player.canAfford(price)) {
                // Get and store the grid position of the clicked tile.
                sf::Vector2i gridPos = uiManager.getTowerMenu().getGridCoord();
                const int tileSize = 64; // Store the tile size.

                // A flag to indicate wether a tower already exists on the selected tile.
                bool towerAlreadyPlaced = false;
                
                // Loop through all existing towers.
                for (const auto& tower : towers) {
                    // Get and store the towers position.
                    sf::Vector2f position = tower.getPosition();
                    // Calculate the towers grid position.
                    sf::Vector2i towerGridPos(position.x / tileSize, position.y / tileSize);

                    // Check if there is a tower already placed at this grid position.
                    if (towerGridPos == gridPos) {
                        // Set the flag to indicate there is a tower already placed. 
                        towerAlreadyPlaced = true;
                        break; // Break to avoid processing unessessary towers.
                    }
                }

                // Check the flag to see if a tower was already placed at the intended grid positon.
                if (towerAlreadyPlaced) {
                    // Print debug
                    std::cout << "A tower already exists at grid postion (" << gridPos.x << ", " << gridPos.y << ")\n";
                    // Close the menu if a tower exists on the tile already.
                    uiManager.getTowerMenu().close(&soundManager);
                } else {
                    // If no tower exists on the tile, construct a new tower at that grid position with specified attributes.
                    sf::Vector2f towerPos(gridPos.x * tileSize + tileSize / 2.0f, gridPos.y * tileSize + tileSize / 2.0f);

                    // Construct a new tower - Params: damagePerShot, burstCount, fireRate, cooldown.
                    towers.emplace_back(towerPos, selectedType, 25.0f, 1, 1.0f, 0.0f);

                    // Take the cost of the tower from the player.
                    player.spend(price);

                    // Print debug
                    std::cout << "Tower Constructed at grid (" << gridPos.x << ", " << gridPos.y << ")\n";
                    uiManager.getTowerMenu().close(&soundManager); // Close the menu when finished.
                }
            } else {
                // If the player cant afford to build the tower, print debug and close the menu.
                sf::Vector2i gridPos = uiManager.getTowerMenu().getGridCoord();
                std::cout << "Can't afford Tower at grid (" << gridPos.x << ", " << gridPos.y << ")\n";
                uiManager.getTowerMenu().close(&soundManager);
            }
        }
    }

    // Update the level manager - handles enemy spawning and wave mechanics.
    levelManager.update(deltaTime, enemies, gameMap.entryPoints);

    // If the current wave is fully cleared create a new harder one.
    if (levelManager.isWaveCleared(enemies)) {
        levelManager.nextWave(); // Prepare the next wave.
    }

    // Update all enemies.
    for (auto& enemy : enemies) {
        enemy->update(deltaTime, 64);
    }

    //Iterate through all enemies and reduce the players life if they reached the pearl.
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if ((*it)->reachedEnd()) { // Check if the enemy reached the end of its path.
            player.loseLife(); // Reduce the players lives by 1.
            //Print debug.
            std::cout << "An enemy reached the pearl. Lives Left: " << player.getLives() << "\n";
            it = enemies.erase(it); // Delete the enemy from existance.
        } else {
            ++it;
        }
    }

    // Check if the players has no lives left. If they dont, generate a new level and cleanup old entities.
    if (player.getLives() <= 0) {
        // Print debug.
        std::cout << "No lives left, generating a new level.\n";
        uiManager.getTowerMenu().close(&soundManager); // Close the tower menu.
        levelManager.generateNewLevel(); // Generate a new lvel.
        enemies.clear(); // Clear old enemies.
        towers.clear(); // Clear old towers.
        player.resetLives(); // Reset the players lives back to 20.
    }

    // Iterate through all towers and update them.
    for (auto& tower : towers) {
        tower.update(deltaTime, enemies, projectiles, &soundManager);
    }

    // Iterate through all projectiles and update them.
    for (auto& projectile : projectiles) {
        projectile.update(deltaTime);
    }

    int oldSize = enemies.size(); // Store the old number of enemies.
    checkCollisions(); // Check for collisions, apply damage, and remove dead enemies.
    int newSize = enemies.size(); // Store the new number of enemies.
    int killsThisFrame = oldSize - newSize; // Count how many kills occured this frame.
    levelManager.addKills(killsThisFrame); // Add kills during the frame to add to the kill counter.

    // Check if the player got any kills.
    if (killsThisFrame > 0) {
        // If they did add the right amount of cha-ching to their money pool.
        float rewardPerKill = 50.0f; // 50 per kill.
        player.earn(rewardPerKill * killsThisFrame); // Give the money to the player.
    }

    // Delete any inactive projectiles.
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(), [](const Projectile& projectile) {
            return !projectile.isActive();
            }),
        projectiles.end()
    );

    // Check if the player clicked the generate new level button.
    if (uiManager.isNewLevelButtonClicked()) {
        uiManager.getTowerMenu().close(&soundManager); // Close the menu.
        levelManager.generateNewLevel(); // Generate a new level.
        enemies.clear(); // Delete old enemies.
        towers.clear(); // Delete old towers.
        // Print debug.
        std::cout << "New level button clicked. Generating a new level.\n";
    }

    // Update the UI manager.
    uiManager.update(deltaTime);
}

// Render the game, draws the map, enemies, towers, UI, and projectiles.
void Game::render() {
    window.clear(sf::Color::Black); // Clear the window.

    // Draw the game map.
    const int tileSize = 64;
    gameMap.draw(window, tileSize);

    // Draw all towers.
    for (const auto& tower : towers) {
        tower.draw(window);
    }

    // Draw all enemies.
    for (const auto& enemy : enemies) {
        enemy->draw(window);
    }

    // Draw all projectiles.
    for (const auto& proj : projectiles) {
        proj.draw(window);
    }

    // Draw all UI elements.
    uiManager.draw(window, player.getMoney(), player.getLives(), levelManager.getCurrentWave(),
        levelManager.getLevelKillCount(), levelManager.getLevelTime(),
        enemies, towers);

    // Check if the game is paused, if it is darkened overlay.
    if (paused) {
        // Get the current view from the window.
        sf::View view = window.getView();
        sf::Vector2f viewSize = view.getSize();

        // Overlay a semi-transparent black rectangle that covers the screen.
        sf::RectangleShape pausedOverlay(viewSize);
        pausedOverlay.setFillColor(sf::Color(0, 0, 0, 150));
        pausedOverlay.setPosition(view.getCenter() - viewSize / 2.f);
        window.draw(pausedOverlay);

        // Draw text to indicate the game is paused in the center of the screen.
        sf::Text pauseText;
        pauseText.setString("Paused");
        pauseText.setCharacterSize(32);
        pauseText.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = pauseText.getLocalBounds();
        pauseText.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
        pauseText.setPosition(view.getCenter());
        window.draw(pauseText);
    }
    // Display the frame on the window.
    window.display();
}

// Close the game and delete anything that needs deleting.
void Game::cleanup() {
    window.close();
}

// The main method.
int main() {
    Game game; // Create the game object.

    // Initialize the game and its resources.
    if (!game.init()) {
        return -1; // if there is an issue terminate the program.
    }
    
    game.run(); // Start the main loop.
    game.cleanup(); // Exit the program.

    return 0; // Indicate successful execution.
}