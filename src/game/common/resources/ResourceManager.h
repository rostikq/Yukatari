//
// Created by user on 3/21/2026.
//

#ifndef YUKATARI_RESOURCEMANAGER_H
#define YUKATARI_RESOURCEMANAGER_H
#include <map>
#include <unordered_map>
#include <vector>

#include "TextureName.h"
#include "SFML/Graphics/Texture.hpp"


class ResourceManager {

    std::map<unsigned short, sf::Texture*> m_textures;

    std::unordered_map<std::string, unsigned short> m_textureNameToId;
    unsigned short m_currentTextureId = 0;

    ResourceManager();
    ~ResourceManager() = default;

    void loadTexture(const std::string& textureName,const char* fileName);



public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    sf::Texture* getTexture(unsigned short id) const {
        return m_textures.at(id);
    }

    sf::Texture* getTexture(const std::string& textureName) const {
        return m_textures.at(m_textureNameToId.at(textureName));
    }

    unsigned short getTextureIdByName(std::string& name) const {
        return m_textureNameToId.at(name);
    }
};


#endif //YUKATARI_RESOURCEMANAGER_H