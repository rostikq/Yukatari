//
// Created by user on 3/23/2026.
//

#ifndef YUKATARI_PACKETINPUTUPDATE_H
#define YUKATARI_PACKETINPUTUPDATE_H
#include "../../common/networking/PacketHeader.h"
#include "SFML/Network/Packet.hpp"


struct PacketInputUpdate {
    PacketHeader header{.type = CS_INPUT_UPDATE};
    signed char x;
    signed char y;

};

inline sf::Packet& operator<<(sf::Packet& packet, const PacketInputUpdate& inputUpdate) {
    packet << inputUpdate.header;
    packet << inputUpdate.x;
    packet << inputUpdate.y;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, PacketInputUpdate& inputUpdate) {
    packet >> inputUpdate.header;
    packet >> inputUpdate.x;
    packet >> inputUpdate.y;
    return packet;
}


#endif //YUKATARI_PACKETINPUTUPDATE_H