//
// Created by user on 3/14/2026.
//

#include "ServerState.h"

#include "../../Debug.h"
#include <iostream>
#include <thread>

#include "../client/networking/PacketInputUpdate.h"
#include "../common/networking/PacketHeader.h"
#include "events/EventEntityTransformUpdate.h"
#include "networking/PacketAttachPlayer.h"
#include "networking/PacketEntitiesLoad.h"
#include "networking/PacketEntityTransformUpdate.h"
#include "networking/PacketWorldLoad.h"

ServerState::ServerState(Application &application) : State(application), m_port(0),
                                                     m_world(), m_camera({m_world.mapSizeY/2.0f, m_world.mapSizeY/2.0f}){

    m_world.addObserver(*this);
    DEBUG_CLOG(this, "Enter port to open: ")
    std::cin >> m_port;
    if (m_udpSocket.bind(m_port) != sf::Socket::Status::Done) {
        DEBUG_CLOG(this, "Binding UDP socket to port " + std::to_string(m_port) + " failed");
    }
    DEBUG_CLOG(this, "Server is created successfully");
    m_udpSocket.setBlocking(false);

    m_window = application.getWindowPtr();

    m_isConnected = true;

    Entity* testEntity = new Entity(m_world, {0,0,0}, 0);
    m_world.addEntity(testEntity);
}

void ServerState::update(float dt) {
    if (!m_isConnected) return;


    m_world.update(dt);

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
                m_clients.back().entityId = m_world.createPlayer();
                m_clients.back().port = recvPort;
                PacketWorldLoad wlPacket;
                wlPacket.mapInfo = m_world.mapInfo;
                sf::Packet sendPacket;
                sendPacket << wlPacket;

                if (m_udpSocket.send(sendPacket, *recvIpAddress, recvPort) == sf::Socket::Status::Done) {
                    DEBUG_CLOG(this, "Sent world data to client " + recvIpAddress->toString());
                }
                sendPacket.clear();
                PacketEntitiesLoad elPacket;
                elPacket.entities = m_world.getEntitySnapshots();
                sendPacket << elPacket;

                if (m_udpSocket.send(sendPacket, *recvIpAddress, recvPort) == sf::Socket::Status::Done) {
                    DEBUG_CLOG(this, "Sent entities data to client " + recvIpAddress->toString());
                }

                sendPacket.clear();

                PacketAttachPlayer apPacket{};
                apPacket.id = m_clients.back().id;

                sendPacket << PacketHeader{.type = SC_ATTACH_PLAYER} << apPacket;
                if (m_udpSocket.send(sendPacket, *recvIpAddress, recvPort) == sf::Socket::Status::Done) {
                    DEBUG_CLOG(this, "Sent player attach event to client" + recvIpAddress->toString());
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
            case PacketType::CS_INPUT_UPDATE: {
                InputInfo input;
                recvPacket >> input.x >> input.y;

                DEBUG_CLOG(this, std::format("{} has sent input update: {} {}", recvIpAddress->toString(), input.x, input.y));
                for (auto& client: m_clients) {
                    if (client.ipAddress == *recvIpAddress) {
                        auto* player = m_world.getPlayer(client.entityId);

                        if (player != nullptr)
                        player->setInput({input.x, input.y});
                    }
                }
            }break;
        }
    }
}

void ServerState::inputProcess(float dt) {
    if (!m_window->hasFocus()) return;
    float speed = 200;
    float cameraScale = m_camera.getScale();

    sf::Vector2f moveVector;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        moveVector.x += -1;
        moveVector.y += 1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        moveVector.x += 1;
        moveVector.y += -1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {

        moveVector.x += -1;
        moveVector.y += -1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){

        moveVector.x += 1;
        moveVector.y += 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Period)) {
        m_camera.setScale(m_camera.getScale() + 0.1 * dt);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Comma)) {
        m_camera.setScale(m_camera.getScale() - 0.1 * dt);
    }
    if (moveVector.lengthSquared() > 1) moveVector = moveVector.normalized();
    m_camera.move(moveVector * dt * speed * cameraScale);
}

void ServerState::render(float dt) {
    if (!m_window) return;

    m_worldRenderer.draw(m_window, m_world, m_camera);

}

ServerState::~ServerState() {
    DEBUG_CLOG(this, "Destroying server");
    m_udpSocket.unbind();
    m_world.removeObserver(*this);
}

void ServerState::onEvent(IEvent &event) {
    if (auto* transformUpdateEvent = dynamic_cast<EventEntityTransformUpdate*>(&event)) {
        for (auto& client: m_clients) {
            sf::Packet sendPacket;
            PacketEntityTransformUpdate elPacket;
            sendPacket << PacketHeader{.type = SC_ENT_TRANSFORM_UPDATE};
            elPacket.posX = transformUpdateEvent->posX;
            elPacket.posY = transformUpdateEvent->posY;
            elPacket.posZ = transformUpdateEvent->posZ;
            elPacket.rotation = transformUpdateEvent->rotation;
            elPacket.id = transformUpdateEvent->id;

            sendPacket << elPacket;

            if (m_udpSocket.send(sendPacket, client.ipAddress, client.port) == sf::Socket::Status::Done) {
                DEBUG_CLOG(this, std::format("Sent transform update info of {} to {}", elPacket.id, client.ipAddress.toString()));
            };
        }
    }
}
