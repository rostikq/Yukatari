//
// Created by user on 3/14/2026.
//

#include "ServerState.h"

#include "../../Debug.h"
#include <iostream>

#include "../common/networking/PacketHeader.h"

ServerState::ServerState(Application &application) : State(application), m_port(0) {
    DEBUG_CLOG(this, "Enter port to open: ")
    std::cin >> m_port;
    if (m_udpSocket.bind(m_port) != sf::Socket::Status::Done) {
        DEBUG_CLOG(this, "Binding UDP socket to port " + std::to_string(m_port) + " failed");
    }
    DEBUG_CLOG(this, "Server is created successfully");
    m_udpSocket.setBlocking(false);

    m_isConnected = true;
}

void ServerState::update(float dt) {
    if (!m_isConnected) return;

    sf::Packet recvPacket;
    std::optional<sf::IpAddress> recvIpAddress;
    while (m_udpSocket.receive(recvPacket, recvIpAddress, m_port) == sf::Socket::Status::Done) {
        PacketHeader header;
        recvPacket >> header;
        if (header.version != CUR_PACKET_VERSION) {
            DEBUG_CLOG(this, std::format("Client {} tried to send outdated packet so it it will be ignored", recvIpAddress->toString()));
            continue;
        }
        switch (header.type) {
            case PacketType::PING:
                DEBUG_CLOG(this, "Received PING packet from client " + recvIpAddress->toString());
                DEBUG_CLOG(this, "Sending PONG to client: " + recvIpAddress->toString());
                sf::Packet sendPacket;
                PacketHeader sendHeader;
                sendHeader.type = PacketType::PING;
                sendPacket << sendHeader;
                   if (m_udpSocket.send(sendPacket, *recvIpAddress, m_port) != sf::Socket::Status::Done) {
                       DEBUG_CLOG(this, "Failed to PONG client.");
                   };

                break;
        }
    }
}

void ServerState::inputProcess(float dt) {

}

void ServerState::render(float dt) {

}

ServerState::~ServerState() {
    DEBUG_CLOG(this, "Destroying server");
    m_udpSocket.unbind();
}

#include "../../Debug.h"

