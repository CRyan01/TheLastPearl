// Name: Conor Ryan
// K-NUM: K00286377

#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <map>

// SoundManager handles loading, storing, and playing sounds.
class SoundManager {
public:
    // Constructor- Initializes the sound manager.
    SoundManager();

    // Loads a sound effect from the specified file and stores it with the given filename.
    void loadSound(const std::string& name, const std::string& filename);

    // Plays the sound effect identified by the given name.
    void playSound(const std::string& name);

    // Plays background music from the specified file.
    void playBackgroundMusic(const std::string& filename, bool loop = true);

    // Stops the background music.
    void stopBackgroundMusic();

private:
    std::map<std::string, sf::SoundBuffer> soundBuffers; // Stores sound buffers by name.
    std::map<std::string, sf::Sound> sounds; // Stores sound objects by name.
    sf::Music backgroundMusic; // Manages background music.
};
