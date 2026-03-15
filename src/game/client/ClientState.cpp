//
// Created by user on 3/15/2026.
//

#include "ClientState.h"

#include "../../Debug.h"
#include <iostream>

#include "../common/events/EventApplicationTerminate.h"
#include "../common/networking/PacketHeader.h"
#include "events/EventWorldLoad.h"

ClientState::ClientState(Application &app) : State(app), m_networkManager(app.getNetworkManager()),
                                             m_serverIp(0,0,0,0),
                                             m_camera(){
    DEBUG_CLOG(this, "Please enter Ip and Port:");

    m_networkManager.addObserver(*this);

    m_window = app.getWindowPtr();

    std::string ipInput;
    std::cin >> ipInput >> m_serverPort;
    m_serverIp = *m_serverIp.resolve(ipInput);

    DEBUG_CLOG(this, "Trying to connect to" + m_serverIp.toString());
    m_networkManager.connect(m_serverIp, m_serverPort);

    sf::Packet packet;
    PacketHeader header;
    header.type = PacketType::CS_CLIENT_CONNECT;
    packet << header;
    m_networkManager.send(packet);
}

ClientState::~ClientState() {
    m_networkManager.disconnect();
    m_networkManager.removeObserver(*this);
}

void ClientState::update(float dt) {
    pingTime -= dt;
    if (pingTime <= 0) {
        sf::Packet packet;
        PacketHeader header;
        header.type = PacketType::PING;
        packet << header;

        m_networkManager.send(packet);
        pingTime = 1.0f;
    }

    m_networkManager.pollPackets();
}

void ClientState::render(float dt) {
    if (!m_window) return;

    m_worldRenderer.draw(m_window, m_world, m_camera);
}

void ClientState::inputProcess(float dt) {
    float speed = 5;

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
    m_camera.move(moveVector * dt * speed);
}

void ClientState::onEvent(IEvent &event) {
    if (auto worldLoadEvent = dynamic_cast<EventWorldLoad*>(&event)) {
        m_world.loadMap(worldLoadEvent->mapInfo);
        m_camera.setPosition({(float)m_world.mapSizeX/2, (float)m_world.mapSizeY/2});
        return;
    }
    if (auto appTerminateEvent = dynamic_cast<EventApplicationTerminate*>(&event)) {
        std::cout << "Received terminate event\n";
        terminate();
    }
}

void ClientState::terminate() {
    sf::Packet packet;
    PacketHeader header;
    header.type = PacketType::CS_CLIENT_DISCONNECT;
    packet << header;
    m_networkManager.send(packet);
}
