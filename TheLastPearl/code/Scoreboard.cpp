// Name: Conor Ryan
// K-NUM: K00286377

#include "Scoreboard.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

// Default constructor - Initializes the scoreboard.
Scoreboard::Scoreboard() {

}

// Load the scores from a text file.
void Scoreboard::loadScores(const std::string& filename) {
	scores.clear(); // Clear old scores.
	std::ifstream file(filename);

	// Check if the text file is open, print debug if its not.
	if (!file.is_open()) {
		std::cout << "Couldnt open the scores file to load.'\n";
		return;
	}

	int score; // Store the players score.

	// Get the scores
	while (file >> score) {
		scores.push_back({ score }); // Add to the list of score.
	}

	file.close(); // Close the file when finished.

	// Sort the scores.
	std::sort(scores.begin(), scores.end(), [](const record& scoreOne, const record& scoreTwo) {
		return scoreOne.score > scoreTwo.score;
	});
}

// Save the scores to a text file.
void Scoreboard::saveScores(const std::string& filename) const {
	std::ofstream file(filename); // Open the file.

	// Check if its open, if not print debug.
	if (!file.is_open()) {
		std::cout << "Couldnt open the scores file to save.'\n";
		return;
	}

	// Save the scores to the file.
	for (const auto& newRecord : scores) {
		file << newRecord.score << "\n";
	}
	file.close(); // Close the file.
}

// Add a score to the scoreboard.
void Scoreboard::addScore(const record& newRecord) {
	scores.push_back(newRecord);

	// Sort the scores.
	std::sort(scores.begin(), scores.end(), [](const record& scoreOne, const record& scoreTwo) {
		return scoreOne.score > scoreTwo.score;
	});

	//Keep only the top 10 scores.
	if (scores.size() > 10) {
		scores.resize(10);
	}
}

void Scoreboard::draw(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2f& position) const {
	sf::Text text; // Create a text object for the scores.

	text.setFont(font); // Set the texts font.
	text.setCharacterSize(18); // Set character size.
	text.setFillColor(sf::Color::White); // Set text color to white.

	// Create an ostringstream to build the scoreboard text.
	std::ostringstream oss;
	oss << "Top 10 Scores:\n";
	int rank = 1; // Start from the highest.

	// Loop through all records and build the string.
	for (const auto& record : scores) {
		oss << rank++ << ". " << record.score << "\n";
	}
	text.setString(oss.str()); // Convert the string stream and set the text.

	// Center the texts origin and postion it.
	sf::FloatRect textBounds = text.getLocalBounds();
	text.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
	text.setPosition(position);

	// Create an overlay for the scoreboard and set its properties.
	float padding = 30.0f; // Menu padding.
	sf::RectangleShape background(sf::Vector2f(textBounds.width + 2 * padding, textBounds.height + 2 * padding));
	background.setFillColor(sf::Color(50, 50, 50, 200));
	background.setOutlineColor(sf::Color(255, 255, 255, 200));
	background.setOutlineThickness(2.0f);
	
	// Center the rectangles origin.
	background.setOrigin((textBounds.width + 2 * padding) / 2, (textBounds.height + 2 * padding) / 2);
	background.setPosition(position); // Set the overlays position.

	// Draw the overlay under the scoreboard text.
	window.draw(background);
	window.draw(text);
}