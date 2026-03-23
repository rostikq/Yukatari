//
// Created by user on 3/14/2026.
//

#ifndef YUKATARI_PACKETHEADER_H
#define YUKATARI_PACKETHEADER_H

#define CUR_PACKET_SIGNATURE "yk"
#define CUR_PACKET_VERSION 1
#include <cstdint>

#include "SFML/Network/Packet.hpp"

enum PacketType : uint8_t {
    PING,
    CS_CLIENT_CONNECT,
    CS_CLIENT_DISCONNECT,
    SC_WORLD_LOAD,
    SC_ENTITIES_LOAD,
    CS_INPUT_UPDATE,
    SC_ENT_TRANSFORM_UPDATE,
    SC_ATTACH_PLAYER
};

struct PacketHeader {
    uint8_t version = CUR_PACKET_VERSION;
    PacketType type;
};

inline sf::Packet& operator<<(sf::Packet& packet, const PacketHeader& header) {
    return packet << header.version << static_cast<uint8_t>(header.type);
}


inline sf::Packet& operator>>(sf::Packet& packet, PacketHeader& header) {
    uint8_t type;
    packet >> header.version >> type;
    header.type = static_cast<PacketType>(type);

    return packet;
}


#endif //YUKATARI_PACKETHEADER_H