//
// Created by user on 3/14/2026.
//

#include "ServerState.h"

#include "../../Debug.h"
#include <iostream>

#include "../common/networking/PacketHeader.h"
#include "networking/PacketWorldLoad.h"

ServerState::ServerState(Application &application) : State(application), m_port(0),
                                                     m_world(), m_camera({m_world.mapSizeY/2.0f, m_world.mapSizeY/2.0f}){

    DEBUG_CLOG(this, "Enter port to open: ")
    std::cin >> m_port;
    if (m_udpSocket.bind(m_port) != sf::Socket::Status::Done) {
        DEBUG_CLOG(this, "Binding UDP socket to port " + std::to_string(m_port) + " failed");
    }
    DEBUG_CLOG(this, "Server is created successfully");
    m_udpSocket.setBlocking(false);

    m_window = application.getWindowPtr();

    m_isConnected = true;
}

void ServerState::update(float dt) {
    if (!m_isConnected) return;

    sf::Packet recvPacket;
    unsigned short recvPort;
    std::optional<sf::IpAddress> recvIpAddress;
    while (m_udpSocket.receive(recvPacket, recvIpAddress, recvPort) == sf::Socket::Status::Done) {
        PacketHeader header;
        recvPacket >> header;
        if (header.version != CUR_PACKET_VERSION) {
            DEBUG_CLOG(this, std::format("Client {} tried to send outdated packet so it it will be ignored", recvIpAddress->toString()));
            continue;
        }
        switch (header.type) {
            case PacketType::PING:
                {
                //DEBUG_CLOG(this, "Received PING packet from client " + recvIpAddress->toString());
                //DEBUG_CLOG(this, "Sending PONG to client: " + recvIpAddress->toString());
                sf::Packet sendPacket;
                PacketHeader sendHeader;
                sendHeader.type = PacketType::PING;
                sendPacket << sendHeader;
                if (m_udpSocket.send(sendPacket, *recvIpAddress, recvPort) != sf::Socket::Status::Done) {
                    DEBUG_CLOG(this, "Failed to PONG client.");
                };
        }
                break;
            case PacketType::CS_CLIENT_CONNECT: {
                DEBUG_CLOG(this, std::format("Client {} attempting to join server", recvIpAddress->toString()));
                m_clients.push_back({*recvIpAddress});
                m_clients.back().id = m_clients.size() - 1;
                PacketWorldLoad wlPacket;
                wlPacket.mapInfo = m_world.mapInfo;
                sf::Packet sendPacket;
                sendPacket << wlPacket;

                if (m_udpSocket.send(sendPacket, *recvIpAddress, recvPort) == sf::Socket::Status::Done) {
                    DEBUG_CLOG(this, "Sent world data to client " + recvIpAddress->toString());
                }
            }
                break;
            case PacketType::CS_CLIENT_DISCONNECT: {
                for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
                    DEBUG_CLOG(this, std::format("Client {} has disconnected from server", recvIpAddress->toString()));
                    if (it->ipAddress == *recvIpAddress) {
                        m_clients.erase(it);
                        break;
                    }
                }
            }
                break;
        }
    }
}

void ServerState::inputProcess(float dt) {
    float speed = 5.0f;

    sf::Vector2f moveVector;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        moveVector.x += -1;
        moveVector.y += 1;
        moveVector = moveVector.normalized();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        moveVector.x += 1;
        moveVector.y += -1;
        moveVector = moveVector.normalized();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {

        moveVector.x += -1;
        moveVector.y += -1;
        moveVector = moveVector.normalized();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){

        moveVector.x += 1;
        moveVector.y += 1;
        moveVector = moveVector.normalized();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Add)) {
        m_camera.setScale(m_camera.getScale() + 0.2 * dt);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Subtract)) {
        m_camera.setScale(m_camera.getScale() - 0.2 * dt);
    }
    m_camera.move(moveVector * dt * speed);
}

void ServerState::render(float dt) {
    if (!m_window) return;

    m_worldRenderer.draw(m_window, m_world, m_camera);

}

ServerState::~ServerState() {
    DEBUG_CLOG(this, "Destroying server");
    m_udpSocket.unbind();
}
