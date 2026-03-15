//
// Created by user on 3/15/2026.
//

#ifndef YUKATARI_TILE_H
#define YUKATARI_TILE_H
#include <cstdint>

enum TileType: uint8_t {
    EMPTY,
    GRASS,
    WATER,
    STONE,
    SAND,
    DIRT
};

struct Tile {
    TileType type = TileType::EMPTY;
};


#endif //YUKATARI_TILE_H