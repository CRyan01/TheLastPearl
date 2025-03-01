// Name: Conor Ryan
// K-NUM: K00286377

#pragma once

#include <vector>
#include <memory>
#include <SFML/System.hpp>
#include "Map.h"
#include "Enemy.h"
#include "EnemyType.h"

class LevelManager {
public:
    // Constructs a levelmanager and passes it a reference to a map object.
    LevelManager(Map& map);

    // Updates the level manager. Passes in the list of enemy pointers and list of entry points for spawning.
    void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<sf::Vector2i>& entryPoints);

    // Returns true if every enemy in the wave is dead.
    bool isWaveCleared(const std::vector<std::shared_ptr<Enemy>>& enemies) const;

    // Prepares the next wave.
    void nextWave();

    // Randomly generates a new level.
    void generateNewLevel();

    // Returns the current wave number.
    int getCurrentWave() const { return currentWave; }

    // Returns the amount of kills the player acheived in the level.
    int getLevelKillCount() const { return currentLevelKillCount; }

    // Returns the amount of time elapsed during the level.
    float getLevelTime() const { return currentLevelTime; }

    // Adds a specified amount of kills to the kill counter.
    void addKills(int kills) { currentLevelKillCount += kills; }

private:
    Map& map;                // The games map.
    int currentWave;         // The current wave number.
    float spawnTimer;        // Time until the next enemy spawn.
    float timeBetweenSpawns; // The delay between spawns.
    int enemiesToSpawn;      // The num of remaining enemies to spawn in the wave.
    size_t nextEntryIndex;   // To alternate selection of the spawn points.

    float baseEnemyHealth; // The enemies base amount of health.
    int baseEnemyCount = 10; // The base number of enemies.
    int enemyCountIncrement = 10; // How many enemies are added with each new wave.
    float baseHealthMultiplier = 1.0f; // The base multiplier of enemy health.
    float healthMultiplierIncrement = 0.1f; // The amount the enemy health muliplier increments each wave.

    float initialTimeBetweenSpawns = 1.0f;   // The starting dealy between indiviual enemy spawns.
    float minTimeBetweenSpawns = 0.16f;         // The minimum delay between individual enemy spawns
    float spawnDecrement = 0.10f;              // The amount the spawn delay decrements between waves.

    int currentLevelKillCount = 0; // The number of kills the player has in the current level.
    float currentLevelTime = 0.0f; // The amount of time the palyer has spent in the current level.

    // Chooses a random enemy type to spawn
    EnemyType chooseEnemyType();
};
