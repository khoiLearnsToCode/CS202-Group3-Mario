#include "ResourceManager.h"
#include <iostream>

// Initialize static instance pointer
ResourceManager* ResourceManager::instance = nullptr;

// Private constructor
ResourceManager::ResourceManager() : loadFromRRES(false) {
    // Initialize empty
}

// Private destructor
ResourceManager::~ResourceManager() {
    unloadResources();
}

// Get singleton instance
ResourceManager& ResourceManager::getInstance() {
    if (instance == nullptr) {
        instance = new ResourceManager();
    }
    return *instance;
}

// Cleanup singleton
void ResourceManager::destroyInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

// Load all resources
void ResourceManager::loadResources() {
    loadTextures();
    loadSounds();
    loadMusics();
}

// Unload all resources
void ResourceManager::unloadResources() {
    unloadTextures();
    unloadSounds();
    unloadMusics();
}

// Get resource maps
std::map<std::string, Texture2D>& ResourceManager::getTextures() {
    return textures;
}

std::map<std::string, Sound>& ResourceManager::getSounds() {
    return sounds;
}

std::map<std::string, Music>& ResourceManager::getMusics() {
    return musics;
}

// Utility getters
//Texture2D& ResourceManager::getTexture(const std::string& key) {
//    return textures[key];
//}
//
//Sound& ResourceManager::getSound(const std::string& key) {
//    return sounds[key];
//}
//
//Music& ResourceManager::getMusic(const std::string& key) {
//    return musics[key];
//}