//
// Created by user on 3/14/2026.
//

#include "ServerState.h"

#include "../../Debug.h"
#include <iostream>

#include "../common/networking/PacketHeader.h"

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

    m_tileShape.setPointCount(4);

    m_tileShape.setPoint(0, {-m_scale/2, 0});
    m_tileShape.setPoint(1, {0, -m_scale/4});
    m_tileShape.setPoint(2, {m_scale/2, 0});
    m_tileShape.setPoint(3, {0, m_scale/4});

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
        m_camera.setScale(m_camera.getScale() + 0.1 * dt);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Subtract)) {
        m_camera.setScale(m_camera.getScale() - 0.1 * dt);
    }
    m_camera.move(moveVector * dt);
}

void ServerState::render(float dt) {
    if (!m_window) return;

    if (m_camera.getScale() != 0)
    m_scale = 1/m_camera.getScale();

    float centerX = m_window->getSize().x / 2.0f;
    float centerY = m_window->getSize().y / 2.0f;

    m_tileShape.setScale({m_scale, m_scale});

    for (int wrldX=0; wrldX<m_world.mapSizeX;++wrldX) {
        for (int wrldY=0; wrldY<m_world.mapSizeY;++wrldY) {
            float aposX = wrldX - m_camera.getPosition().x;
            float aposY = wrldY - m_camera.getPosition().y;

            float projectedX = (aposX - aposY) * m_scale / 2.0f;
            float projectedY = (aposX + aposY) * m_scale / 4.0f;

            projectedX += centerX;
            projectedY += centerY;

            m_tileShape.setPosition({projectedX, projectedY});

            sf::Color color;
            switch (m_world.map[wrldY * m_world.mapSizeY + wrldX]) {
                case EMPTY:
                    color = sf::Color::White;
                    break;
                case GRASS:
                    color = sf::Color::Green;
                    break;
                case WATER:
                    color = sf::Color::Blue;
                    break;
                case SAND:
                    color = sf::Color::Yellow;
                    break;
                default:
                    color = sf::Color::White;
                    break;
            }

            m_tileShape.setFillColor(color);

            m_window->draw(m_tileShape);

        }
    }
}

ServerState::~ServerState() {
    DEBUG_CLOG(this, "Destroying server");
    m_udpSocket.unbind();
}
