//
// Created by user on 3/15/2026.
//

#ifndef YUKATARI_WORLD_H
#define YUKATARI_WORLD_H
#include <vector>

#include "SFML/System/Vector2.hpp"
#include "world/MapInfo.h"
#include "world/Tile.h"


class World {
public:
    World();
    ~World();

    size_t mapSizeX = 128;
    size_t mapSizeY = 128;
    TileType *map = nullptr;
    std::vector<sf::Vector2i> spawnTiles;

    MapInfo mapInfo;

    void loadMap(MapInfo &mapInfo);

    MapInfo getMapInfo();

};


#endif //YUKATARI_WORLD_H