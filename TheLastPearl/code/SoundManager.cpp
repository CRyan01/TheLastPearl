// Name: Conor Ryan
// K-NUM: K00286377

#include "SoundManager.h"
#include <iostream>

// Constructor initializes the sound manager.
SoundManager::SoundManager() {}

// Loads a sound from a specified file and stores it under a specified filename.
void SoundManager::loadSound(const std::string& name, const std::string& filename) {
    sf::SoundBuffer buffer; // Create a sound buffer.
    // Try to load the sound file.
    if (!buffer.loadFromFile(filename)) {
        // Print debug & return if theres a problem.
        std::cout << "Couldnt load sound: " << filename << std::endl;
        return;
    }

    // Store the sound buffer in the map using the name as a key.
    soundBuffers[name] = buffer;

    // Create a instance of a sound object.
    sf::Sound sound;

    // Set the buffer to the new one.
    sound.setBuffer(soundBuffers[name]);

    // Set the sounds volume to 7%.
    sound.setVolume(7.0f);

    // Store the sound in the sound map for later use.
    sounds[name] = sound;
}

// Plays the sound stored under a specific name.
void SoundManager::playSound(const std::string& name) {
    // Check if the sound exists in the map.
    if (sounds.find(name) != sounds.end()) {
        sounds[name].play(); // If its found play the sound.
    }
    else {
        std::cout << "Sound could not be found: " << name << std::endl;
    }
}

// Opens the background track from a file, sets looping as sepcified and being playing it.
void SoundManager::playBackgroundMusic(const std::string& filename, bool loop) {
    // Try to open the background file.
    if (!backgroundMusic.openFromFile(filename)) {
        // Print debug if the file fails to load.
        std::cerr << "Couldnt load background music: " << filename << std::endl;
        return;
    }
    backgroundMusic.setLoop(loop); // Set the track to loop if specified.
    backgroundMusic.setVolume(30.f); // Set volume to 30%
    backgroundMusic.play(); // Play the track.
}

// Stops the currently playing background music.
void SoundManager::stopBackgroundMusic() {
    backgroundMusic.stop(); // Stop the track.
}