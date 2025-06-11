#pragma once

#include "raylib.h"
#include <map>
#include <string>

class ResourceManager {

private:
    // Static instance pointer
    static ResourceManager* instance;

    // Instance data members (no longer static)
    std::map<std::string, Texture2D> textures;
    std::map<std::string, Sound> sounds;
    std::map<std::string, Music> musics;

    bool loadFromRRES;

    // Private constructor (prevents external instantiation)
    ResourceManager();

    // Private destructor
    ~ResourceManager();

    // Delete copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Private helper methods
    void loadTextures();
    void loadSounds();
    void loadMusics();

    void loadTexture(const std::string& key, const std::string& path);
    void loadSound(const std::string& key, const std::string& path);
    void loadMusic(const std::string& key, const std::string& path);

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
    /*Texture2D& getTexture(const std::string& key);
    Sound& getSound(const std::string& key);
    Music& getMusic(const std::string& key);*/
};

// In .cpp file, do sth that looks like this:

//#include "ResourceManager.h"
//#include <iostream>
//
//// Initialize static instance pointer
//ResourceManager* ResourceManager::instance = nullptr;
//
//// Private constructor
//ResourceManager::ResourceManager() : loadFromRRES(false) {
//    // Initialize empty
//}
//
//// Private destructor
//ResourceManager::~ResourceManager() {
//    unloadResources();
//}
//
//// Get singleton instance
//ResourceManager& ResourceManager::getInstance() {
//    if (instance == nullptr) {
//        instance = new ResourceManager();
//    }
//    return *instance;
//}
//
//// Cleanup singleton
//void ResourceManager::destroyInstance() {
//    if (instance != nullptr) {
//        delete instance;
//        instance = nullptr;
//    }
//}
//
//// Load all resources
//void ResourceManager::loadResources() {
//    loadTextures();
//    loadSounds();
//    loadMusics();
//}
//
//// Unload all resources
//void ResourceManager::unloadResources() {
//    unloadTextures();
//    unloadSounds();
//    unloadMusics();
//}
//
//// Get resource maps
//std::map<std::string, Texture2D>& ResourceManager::getTextures() {
//    return textures;
//}
//
//std::map<std::string, Sound>& ResourceManager::getSounds() {
//    return sounds;
//}
//
//std::map<std::string, Music>& ResourceManager::getMusics() {
//    return musics;
//}
//
//// Utility getters
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