//
// Created by user on 3/15/2026.
//

#include "World.h"

#include <complex>
#include <format>
#include <iostream>

#include "../../PerlinNoise.hpp"
#include "../../Debug.h"

#define MAP_SEED 696969696969;

World::World() {
    map = new TileType[mapSizeY * mapSizeX];
    DEBUG_CLOG(this, std::format("{} bytes allocated for map", mapSizeY * mapSizeX));

    const siv::PerlinNoise::seed_type seed = MAP_SEED;

    const siv::PerlinNoise perlin {seed};

    float grassHeight = 0.65;
    float sandHeight = 0.6;

    size_t spawnCounter = 0;

    for (int y = 0; y < mapSizeY; y++) {
        for (int x = 0; x < mapSizeX; x++) {

            float height = perlin.octave2D_01((x * 0.01), (y* 0.01), 4);
            if (height > grassHeight ) {
                map[mapSizeY * y + x] = TileType::GRASS;
                spawnCounter++;
            }
            else if (height > sandHeight) {
                map[mapSizeY * y + x] = TileType::SAND;
                spawnCounter++;
            }
            else {
                map[mapSizeY * y + x] = TileType::WATER;
            }

            if (spawnCounter == 100) {
                spawnCounter = 0;
                spawnTiles.emplace_back(x, y);
            }

        }
    }

    mapInfo.mapSizeX = mapSizeX;
    mapInfo.mapSizeY = mapSizeY;
    for (size_t i = 0; i < mapSizeY * mapSizeX; i++) {
        mapInfo.info.push_back((uint8_t)map[i]);
    }
}

MapInfo World::getMapInfo() {
    mapInfo.info.resize(mapSizeY * mapSizeX);
    for (size_t i = 0; i < mapSizeY * mapSizeX; i++) {
        mapInfo.info.at(i) = (uint8_t)map[i];
    }
    return mapInfo;
}

World::~World() {
    delete map;
}

void World::loadMap(MapInfo &mapInfo) {
    mapSizeY = mapInfo.mapSizeY;
    mapSizeX = mapInfo.mapSizeX;
    delete map;
    map = new TileType[mapSizeY * mapSizeX];
    for (size_t i = 0; i < mapSizeY * mapSizeX; i++) {
        map[i] = (TileType)mapInfo.info[i];
    }
}
