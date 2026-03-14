//
// Created by user on 3/14/2026.
//

#include "NetworkManager.h"

#include <iostream>

#include "../Debug.h"
#include "SFML/Network/Packet.hpp"

NetworkManager::NetworkManager() :  m_port(0) {
    m_socket.setBlocking(false);
    DEBUG_CLOG(this, "Network Manager created successfully");
}

void NetworkManager::connect(const char *ipAddress, int port) {
    if (m_socket.bind(port) != sf::Socket::Status::Done) {
        DEBUG_CLOG(this, "Binding socket failed");
        return;
    }
    DEBUG_CLOG(this, std::format("Connecting successfully to {}:{}", ipAddress, port));
    m_serverIp = sf::IpAddress::resolve(ipAddress);
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

    std::optional<sf::IpAddress> sender;
    while (m_socket.receive(packet,  sender, m_port) == sf::Socket::Status::Done) {

    }
}
