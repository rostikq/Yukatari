//
// Created by user on 3/21/2026.
//

#include "ResourceManager.h"

#include "../../../Debug.h"

#include <fstream>

#include "TextureName.h"
#define fs std::filesystem
#define RS_TEXTURES_PATH fs::current_path().parent_path().append("resources\\textures\\")

ResourceManager::ResourceManager() {
    DEBUG_CLOG(this, "ResourceManager has been initialized");

    DEBUG_CLOG(this, "Loading textures...");
    loadTexture("tilemap_dev", "tilemap_dev.png");
    loadTexture("tilemap_world", "tilemap_game.png");
    loadTexture("tilemap_world_lod", "tilemap_game_lod.png");
}

void ResourceManager::loadTexture(const std::string& textureName, const char *fileName) {
    unsigned short id = m_currentTextureId++;
    m_textureNameToId[textureName] = id;
    m_textures[id] = new sf::Texture(RS_TEXTURES_PATH.append(fileName));
    if (m_textures[id] != nullptr) {
        DEBUG_CLOG(this, std::format("Texture {} has loaded successfully", textureName));
    }
}
