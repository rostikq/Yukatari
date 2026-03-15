//
// Created by user on 3/14/2026.
//

#include "NetworkManager.h"

#include <iostream>

#include "../Debug.h"
#include "../game/common/networking/PacketHeader.h"
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
                DEBUG_CLOG(this, "PONG packet received from server!");
            }
                break;
        }
    }
}
