// Name: Conor Ryan
// K-NUM: K00286377

#pragma once

#include <SFML/Graphics.hpp>
#include "UIManager.h"
#include <functional>

// The input manager handles window input and passes UI events to the UI manager, as well as ingame events.
class InputManager {
public:
    // Constructor - Constructs the input manager using a reference to the UI manager.
    InputManager(UIManager& uiManager);

    // Processes all input events in the specified window.
    void processInput(sf::RenderWindow& window);

    // A callback to handle in-game events
    void setGameEventHandler(const std::function<void(const sf::Event&)>& handler);

private:
    UIManager& uiManager;  // A reference to the UI manager.
    std::function<void(const sf::Event&)> gameEventHandler;  // A callback for game events.
};
