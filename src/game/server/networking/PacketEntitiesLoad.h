//
// Created by user on 3/18/2026.
//

#ifndef YUKATARI_PACKETENTITIESLOAD_H
#define YUKATARI_PACKETENTITIESLOAD_H
#include "../../common/networking/PacketHeader.h"
#include "../../common/world/Entity.h"


struct PacketEntitiesLoad{
    PacketHeader header{
        .type = SC_ENTITIES_LOAD
    };
    std::vector<EntitySnapshot> entities;
};

inline sf::Packet& operator<<(sf::Packet& packet, const PacketEntitiesLoad& packetEntitiesLoad) {
    packet << packetEntitiesLoad.header;
    packet << packetEntitiesLoad.entities.size();
    if (!packetEntitiesLoad.entities.empty()) {
        packet.append(packetEntitiesLoad.entities.data(), packetEntitiesLoad.entities.size());
    }

    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, PacketEntitiesLoad& packetEntitiesLoad) {
    packet >> packetEntitiesLoad.header;
    size_t size;
    packet >> size;
    packetEntitiesLoad.entities.resize(size);
    if (!packetEntitiesLoad.entities.empty()) {
        for (size_t i = 0; i < size; ++i) {
            packet >> packetEntitiesLoad.entities[i];
        }
    }
    return packet;
}


#endif //YUKATARI_PACKETENTITIESLOAD_H