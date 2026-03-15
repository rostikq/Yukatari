//
// Created by user on 3/15/2026.
//

#ifndef YUKATARI_WORLDINFO_H
#define YUKATARI_WORLDINFO_H

#include <cstdint>
#include <vector>

#include "SFML/Network/Packet.hpp"

struct MapInfo {
    size_t mapSizeX;
    size_t mapSizeY;
    std::vector<uint8_t> info;
};

inline sf::Packet& operator<<(sf::Packet& packet, const MapInfo& mapInfo) {
    packet << mapInfo.mapSizeX << mapInfo.mapSizeY;
    size_t size = mapInfo.info.size();
    packet << size;
    if (size > 0) {
        packet.append(mapInfo.info.data(), size);
    }
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, MapInfo& mapInfo) {
    packet >> mapInfo.mapSizeX >> mapInfo.mapSizeY;
    size_t size;
    packet >> size;

    mapInfo.info.resize(size);

    for (size_t i = 0; i < size; ++i) {
        packet >> mapInfo.info[i];
    }
    return packet;
}


#endif //YUKATARI_WORLDINFO_H