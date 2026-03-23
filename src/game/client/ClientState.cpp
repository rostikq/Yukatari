//
// Created by user on 3/15/2026.
//

#include "ClientState.h"

#include "../../Debug.h"
#include <iostream>

#include "../common/events/EventApplicationTerminate.h"
#include "../common/networking/PacketHeader.h"
#include "../server/events/EventEntityTransformUpdate.h"
#include "../server/networking/PacketEntitiesLoad.h"
#include "events/EventAttachPlayer.h"
#include "events/EventEntitiesLoad.h"
#include "events/EventWorldLoad.h"
#include "networking/PacketInputUpdate.h"

ClientState::ClientState(Application &app) : State(app), m_networkManager(app.getNetworkManager()),
                                             m_serverIp(0,0,0,0),
                                             m_camera(),
                                             m_lastInputInfo(0,0){
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
    m_camera.setFollowEntity(m_playerEntity);

    m_camera.update(dt);

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
    float speed = 200;
    float cameraScale = m_camera.getScale();

    InputInfo currentMove {0,0};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        currentMove.x += -1;
        currentMove.y += 1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        currentMove.x += 1;
        currentMove.y += -1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {

        currentMove.x += -1;
        currentMove.y += -1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){

        currentMove.x += 1;
        currentMove.y += 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Period)) {
        m_camera.setScale(m_camera.getScale() + 0.1 * dt);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Comma)) {
        m_camera.setScale(m_camera.getScale() - 0.1 * dt);
    }

    if (currentMove != m_lastInputInfo) {
        m_lastInputInfo = currentMove;
        PacketInputUpdate inputUpdate;
        inputUpdate.x = currentMove.x;
        inputUpdate.y = currentMove.y;
        sf::Packet packet;
        packet << inputUpdate;
        std::cout << (int)inputUpdate.x << " " << (int)inputUpdate.y << "\n";
        m_networkManager.send(packet);
    }
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
    if (auto entityLoadEvent = dynamic_cast<EventEntitiesLoad*>(&event)) {
        std::cout << "Received entity load event\n";
        m_world.loadEntities(entityLoadEvent->snapshots);
    }
    if (auto entityTransformUpdateEvent = dynamic_cast<EventEntityTransformUpdate*>(&event)) {
        std::cout << "Received entity transform update event\n";
        if (Entity* ent = m_world.getEntity(entityTransformUpdateEvent->id)) {
            ent->setPosition({entityTransformUpdateEvent->posX,
            entityTransformUpdateEvent->posY,
            entityTransformUpdateEvent->posZ});
            ent->setRotation(entityTransformUpdateEvent->rotation);
        }
    }
    if (auto playerAttachEvent = dynamic_cast<EventAttachPlayer*>(&event)) {
        std::cout << "Received player attach event\n";
        if (Entity* ent = m_world.getEntity(playerAttachEvent->id)) {
            m_playerEntity = ent;
        }
    }
}

void ClientState::terminate() {
    sf::Packet packet;
    PacketHeader header;
    header.type = PacketType::CS_CLIENT_DISCONNECT;
    packet << header;
    m_networkManager.send(packet);
}
