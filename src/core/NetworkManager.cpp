//
// Created by user on 3/14/2026.
//

#include "NetworkManager.h"

#include <iostream>

#include "../Debug.h"
#include "../game/client/events/EventAttachPlayer.h"
#include "../game/client/events/EventEntitiesLoad.h"
#include "../game/client/events/EventWorldLoad.h"
#include "../game/common/networking/PacketHeader.h"
#include "../game/common/world/Entity.h"
#include "../game/common/world/MapInfo.h"
#include "../game/server/events/EventEntityTransformUpdate.h"
#include "../game/server/networking/PacketAttachPlayer.h"
#include "../game/server/networking/PacketEntitiesLoad.h"
#include "../game/server/networking/PacketEntityTransformUpdate.h"
#include "SFML/Network/Packet.hpp"

NetworkManager::NetworkManager() :  m_port(0) {
    m_socket.setBlocking(false);
    DEBUG_CLOG(this, "Network Manager created successfully");
}

void NetworkManager::connect(sf::IpAddress ipAddress, unsigned int port) {
    if (m_socket.bind(port) != sf::Socket::Status::Done) {
        DEBUG_CLOG(this, "Binding socket failed. Probably, port is already opened?");
    }
    DEBUG_CLOG(this, std::format("Connecting successfully to {}:{}", ipAddress.toString(), port));
    m_socket.setBlocking(false);
    m_serverIp = ipAddress;
    m_port = port;
    m_isConnected = true;
}

void NetworkManager::onEvent(IEvent &event) {

}

void NetworkManager::send(sf::Packet& packet) {
    if (!m_isConnected) return;
    if (m_socket.send(packet, *m_serverIp, m_port) != sf::Socket::Status::Done) {
        DEBUG_CLOG(this, "Packet send failed");
    };
}

void NetworkManager::disconnect() {
    m_socket.unbind();
    m_isConnected = false;
    DEBUG_CLOG(this, "Disconnected successfully");
}

void NetworkManager::pollPackets() {
    if (!m_isConnected) return;
    sf::Packet packet;

    unsigned short port;
    std::optional<sf::IpAddress> sender;
    while (true) {
        packet.clear();

        if (m_socket.receive(packet, sender, port) != sf::Socket::Status::Done) {
            break;
        }

        PacketHeader header;
        packet >> header;
        switch (header.type) {
            case PacketType::PING: {
                //DEBUG_CLOG(this, "PONG packet received from server!");
            }
                break;
            case PacketType::SC_WORLD_LOAD: {
                MapInfo mapInfo;
                packet >> mapInfo;
                DEBUG_CLOG(this, "SC_WORLD_LOAD packet received from server!");
                EventWorldLoad wlEvent;
                wlEvent.mapInfo = mapInfo;
                publish(wlEvent);
            }break;
            case PacketType::SC_ENTITIES_LOAD: {
                PacketEntitiesLoad snapshotPacket;
                packet << header;
                packet >> snapshotPacket;

                DEBUG_CLOG(this, "SC_ENTITIES_LOAD packet received from server!");
                EventEntitiesLoad elEvent;
                elEvent.snapshots = snapshotPacket.entities;
                publish(elEvent);
            }break;
            case PacketType::SC_ENT_TRANSFORM_UPDATE: {
                PacketEntityTransformUpdate transformUpdate;
                packet >> transformUpdate;
                EventEntityTransformUpdate etuEvent;
                etuEvent.id = transformUpdate.id;
                etuEvent.posX    = transformUpdate.posX;
                etuEvent.posY    = transformUpdate.posY;
                etuEvent.posZ    = transformUpdate.posZ;
                etuEvent.rotation = transformUpdate.rotation;
                publish (etuEvent);
                DEBUG_CLOG(this, "SC_ENT_TRANSFORM_UPDATE packet received from server!");
            }break;
            case PacketType::SC_ATTACH_PLAYER: {
                PacketAttachPlayer apPacket {};
                packet >> apPacket;
                EventAttachPlayer apEvent;
                apEvent.id = apPacket.id;
                publish(apEvent);
            }break;
        }
    }
}
