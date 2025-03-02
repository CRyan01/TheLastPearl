// Name: Conor Ryan
// K-NUM: K00286377

#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

struct record {
	int score; // The players score.
};

class Scoreboard {
public:
	// Default constructor for the scoreboard.
	Scoreboard();

	// Load the score from a text file.
	void loadScores(const std::string& filename);

	// Save the scores to a text file.
	void saveScores(const std::string& filename) const;

	// Add a new score and sort the list of scores.
	void addScore(const record& newRecord);

	// Draw the scoreboard at a given position.
	void draw(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2f& position) const;
private:
	std::vector<record> scores; // A vector of score records.
};
