//
// Created by user on 3/23/2026.
//

#ifndef YUKATARI_PACKETENTITYTRANSFORMUPDATE_H
#define YUKATARI_PACKETENTITYTRANSFORMUPDATE_H


struct PacketEntityTransformUpdate {
    unsigned int id = 0;
    float posX = 0;
    float posY = 0;
    float posZ = 0;
    float rotation = 0;
};

inline sf::Packet& operator<<(sf::Packet& packet, const PacketEntityTransformUpdate& data) {
    packet << data.id;
    packet << data.posX;
    packet << data.posY;
    packet << data.posZ;
    packet << data.rotation;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, PacketEntityTransformUpdate& data) {
    packet >> data.id;
    packet >> data.posX;
    packet >> data.posY;
    packet >> data.posZ;
    packet >> data.rotation;
    return packet;
}


#endif //YUKATARI_PACKETENTITYTRANSFORMUPDATE_H