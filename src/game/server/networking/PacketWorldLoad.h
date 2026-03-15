//
// Created by user on 3/15/2026.
//

#ifndef YUKATARI_PACKETWORLDLOAD_H
#define YUKATARI_PACKETWORLDLOAD_H
#include "../../common/networking/PacketHeader.h"
#include "../../common/world/MapInfo.h"


struct PacketWorldLoad {
    PacketHeader header{
        .type = SC_WORLD_LOAD
    };
    MapInfo mapInfo;
};


inline sf::Packet& operator<<(sf::Packet& packet, const PacketWorldLoad& packetWorldLoad) {
    return packet << packetWorldLoad.header << packetWorldLoad.mapInfo;
}

inline sf::Packet& operator>>(sf::Packet& packet, PacketWorldLoad& packetWorldLoad) {
    return packet >> packetWorldLoad.header >> packetWorldLoad.mapInfo;
}

#endif //YUKATARI_PACKETWORLDLOAD_H