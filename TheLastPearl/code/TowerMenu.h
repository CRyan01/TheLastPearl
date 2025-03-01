#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// A struct which represents an options in the tower menu.
struct TowerOption {
    std::string name;  // The name of the tower option.
    float price;       // The price to build the tower.
};

// The tower menu class is repsonsible for displaying a menu of tower build
// options when the player clicks a tile.
class TowerMenu {
public:
    // Default contructor for the tower menu.
    TowerMenu();

    // Opens the menu at the specified grid position.
    void open(const sf::Vector2i& gridPos, const sf::Vector2u& windowSize);

    // Handles clicks within the tower menu, stores the selected option.
    void handleInput(const sf::Event& event, const sf::Vector2f& worldPos);
    
    // Returns the option the player selected then resets it.
    int getSelectedOption();

    // Draws the menu on the  SFML window.
    void draw(sf::RenderWindow& window) const;

    // Returns true if the menu is open.
    bool isOpen() const;

    // Closes the menu.
    void close();

    // Returns the top left pixel position of the menu.
    sf::Vector2i getPosition() const;

    // Returns the grid coordinate where the menu was opened.
    sf::Vector2i getGridCoord() const;

    // Returns the menus width.
    int getMenuWidth() const { return menuWidth; }

    // Returns the menus height, calculated based on the number of options.
    int getMenuHeight() const { return static_cast<int>(options.size()) * buttonHeight; }

private:
    bool openFlag;               // A flag to indicate if the menu is open.
    sf::Vector2i position;       // The top left pixel position of the menu.
    sf::Vector2i gridCoord;      // The grid coordinates of tile the menu was opened from.
    std::vector<TowerOption> options;   // A vector of tower options.
    sf::Font font;               // Font used for the menus text.
    std::vector<sf::Text> optionTexts;  // Text objects for each tower option.
    int selectedOption;          // The selected option index. -1 means nothing was selected.

    const int menuWidth = 300;   // Menu width in pixels.
    const int buttonHeight = 50; // Height of each row.
};
