#pragma once

#include "raylib.h"
#include "utils.h"
#include <map>
#include <vector>
#include <string>

class ResourceManager {

private:
    // Static instance pointer
    static ResourceManager* instance;

    std::map<std::string, Texture2D> textures;
    std::map<std::string, Sound> sounds;
    std::map<std::string, Music> musics;

    // Private constructor (prevents external instantiation)
    ResourceManager() = default;

    // Private destructor
    ~ResourceManager() = default;

    // Delete copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Private helper methods
    void loadTextures();
    void loadSounds();
    void loadMusics();

    // void loadTexture(const std::string& key, const std::string& path);
    // void loadSound(const std::string& key, const std::string& path);
    // void loadMusic(const std::string& key, const std::string& path);

    void unloadTextures();
    void unloadSounds();
    void unloadMusics();

public:
    // Static method to get the singleton instance
    static ResourceManager& getInstance();

    // Static cleanup method
    static void destroyInstance();

    // Public interface methods (no longer static)
    void loadResources();
    void unloadResources();

    std::map<std::string, Texture2D>& getTextures();
    std::map<std::string, Sound>& getSounds();
    std::map<std::string, Music>& getMusics();

    // Utility methods
    Texture2D& getTexture(const std::string& key);
    Sound& getSound(const std::string& key);
    Music& getMusic(const std::string& key);
};
