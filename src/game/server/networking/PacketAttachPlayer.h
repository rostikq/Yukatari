//
// Created by user on 3/23/2026.
//

#ifndef YUKATARI_PACKETATTACHPLAYER_H
#define YUKATARI_PACKETATTACHPLAYER_H


struct PacketAttachPlayer {
    unsigned int id;
};

inline sf::Packet& operator<<(sf::Packet& packet, const PacketAttachPlayer& packetAttachPlayer) {
    return packet << packetAttachPlayer.id;
}

inline sf::Packet& operator>>(sf::Packet& packet, PacketAttachPlayer& packetAttachPlayer) {
    return packet >> packetAttachPlayer.id;
}


#endif //YUKATARI_PACKETATTACHPLAYER_H