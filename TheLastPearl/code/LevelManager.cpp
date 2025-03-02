// Name: Conor Ryan
// K-NUM: K00286377

#include "LevelManager.h"
#include "Pathfinder.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Parameterized constructor - Initializes the level manager with a referenced to the game map.
LevelManager::LevelManager(Map& map)
    : map(map),
    currentWave(1),
    spawnTimer(0.0f),
    timeBetweenSpawns(initialTimeBetweenSpawns),
    enemiesToSpawn(baseEnemyCount),
    nextEntryIndex(0),
    baseEnemyHealth(100.0f)
{
    std::srand(std::time(nullptr));  // Seed the random number generator.
}

// Updates the games state.
void LevelManager::update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<sf::Vector2i>& entryPoints) {
    currentLevelTime += deltaTime; // Store time elapsed in the current level.
    spawnTimer -= deltaTime;     // Decrease the spawn timer.

    // Spawn new enemies when its time and there are enemies left.
    if (spawnTimer <= 0 && enemiesToSpawn > 0) {
        if (entryPoints.empty()) { // If there are no entry points exit the loop.
            return;
        }

        // Select an entry point to spawn the enemies from in and cycle through.
        sf::Vector2i gridPos = entryPoints[nextEntryIndex];
        nextEntryIndex = (nextEntryIndex + 1) % entryPoints.size();
        
        // Convert the grid postion back into a world positon.
        int tileSize = 64;
        sf::Vector2f spawnPos(gridPos.x * tileSize + tileSize / 2.0f,
            gridPos.y * tileSize + tileSize / 2.0f);

        Pathfinder pf; // Create a pathfinder instance.

        // Find a path from the entry tile to the target tile.
        std::vector<sf::Vector2i> path = pf.findPath(map, gridPos, map.targetPoint);

        // Check if the path is a valid path before spawning the enemy.
        if (!path.empty()) {
            // Calculate the enemies health based on the current health multiplier.
            float currentMultiplier = baseHealthMultiplier + (currentWave - 1) * healthMultiplierIncrement;
            float enemyHealth = baseEnemyHealth * currentMultiplier;

            // Create the enemy and add it to the vector of smart pointers.
            std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(spawnPos, path, chooseEnemyType(), enemyHealth);
            enemies.push_back(enemy);

            // Reduce the number of remaining enemies.
            enemiesToSpawn--;
        }

        // Reset the spawn timer.
        spawnTimer = timeBetweenSpawns;
    }
}

// Check if the wave is fully cleared.
bool LevelManager::isWaveCleared(const std::vector<std::shared_ptr<Enemy>>& enemies) const {
    // Check there are enemies left to spawn.
    if (enemiesToSpawn > 0) {
        return false; // Return false if there are enemies left to spawn.
    }

    // Check if any enemies are alive.
    for (const auto& enemy : enemies) {
        if (enemy->isAlive()) {
            return false; // Return false if there are enem ies still alive.
        }
    }
    return true; // Return true if all enemies are dead.
}

// Prepares and starts the next wave.
void LevelManager::nextWave() {
    // Increase the wave counter.
    currentWave++;

    // Calculate a new higher enemy count based on 'enemyCountIncrement' for the new wave.
    int newEnemyCount = baseEnemyCount + (currentWave - 1) * enemyCountIncrement;
    enemiesToSpawn = newEnemyCount;

    spawnTimer = 0.0f; // Reset the spawn timer.
    nextEntryIndex = 0; // Reset the entry index.

    // With each new wave decrease the spawn delay while keeping it within 'minTimeBetweenSpawns' milliseconds.
    timeBetweenSpawns = std::max(minTimeBetweenSpawns, initialTimeBetweenSpawns - (currentWave - 1) * spawnDecrement);

    // Print debug
    float currentMultiplier = baseHealthMultiplier + (currentWave - 1) * healthMultiplierIncrement;
    std::cout << "Wave: " << currentWave << ", Num of Enemies: " << enemiesToSpawn << ", Health Multiplier: " << currentMultiplier << ", Spawn Delay: " << timeBetweenSpawns<< std::endl;
}

// Generates a new level and resets relevant variables.
void LevelManager::generateNewLevel() {
    map.generateRandom(); // Generate a new map.
    currentWave = 1; // Reset the wave to 1.
    enemiesToSpawn = baseEnemyCount; // Restore the base enemy amount.
    spawnTimer = 0.0f; // Reset the spawn timer.
    nextEntryIndex = 0; // Reset the spawn index.
    currentLevelKillCount = 0; // Reset the kill counter.
    currentLevelTime = 0.0f; // Reset the level timer.
    timeBetweenSpawns = initialTimeBetweenSpawns; // Restore the inital time between spawns.
}

// Randomly selects an enemy type to spawn.
EnemyType LevelManager::chooseEnemyType() {
    int r = std::rand() % 4; // Generate a random number between 0 and 3.

    // Return  a enum type of which enemy to spawn.
    switch (r) {
        case 0:
            // Marauder enemy.
            return EnemyType::Marauder;
            break;
        case 1:
            // Privateer enemy.
            return EnemyType::Privateer;
            break;
        case 2:
            //Corsair enemy.
            return EnemyType::Corsair;
            break;
        case 3:
            // Captain enemy.
            return EnemyType::Captain;
            break;
        default:
            // Default to a marauder enemy.
            return EnemyType::Marauder;
    }
}
