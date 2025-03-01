#pragma once
#include "TowerType.h"
#include "EnemyType.h"

// Calculates the damage multiplier based on the tower and enemy types.
inline float calculateDamageMultiplier(TowerType towerType, EnemyType enemyType) {
    if (towerType == TowerType::Pistol && enemyType == EnemyType::Marauder)
        return 1.5f;
    else if (towerType == TowerType::Rifle && enemyType == EnemyType::Privateer)
        return 1.5f;
    else if (towerType == TowerType::Cannon && enemyType == EnemyType::Corsair)
        return 1.5f;
    else if (towerType == TowerType::Carronade && enemyType == EnemyType::Captain)
        return 1.5f;
    else
        return 1.0f;
}
