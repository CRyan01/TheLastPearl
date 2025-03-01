// Name: Conor Ryan
// K-NUM: K00286377

#include "InputManager.h"

// Constructor - initializes the input manager and pass a reference to the UI manager.
InputManager::InputManager(UIManager& uiManager)
    : uiManager(uiManager)
{}

// Sets up the event hander for game events.
void InputManager::setGameEventHandler(const std::function<void(const sf::Event&)>& handler) {
    gameEventHandler = handler;
}

// Process user inputs and handle events.
void InputManager::processInput(sf::RenderWindow& window) {
    sf::Event event; // Declare an event.

    // Capture events from the window.
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close(); // Close the window if the 'X' in the top right is clicked.
        }

        // Pass the event to the UI manager for handling UI events.
        uiManager.processEvent(event, window);

        // Check if the game event handler is set.
        if (gameEventHandler) {
            // Call the game event handler and pass it the current event.
            gameEventHandler(event);
        }
    }
}