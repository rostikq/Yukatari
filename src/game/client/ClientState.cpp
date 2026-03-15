//
// Created by user on 3/15/2026.
//

#include "ClientState.h"

#include "../../Debug.h"
#include <iostream>

#include "../common/networking/PacketHeader.h"

ClientState::ClientState(Application &app) : State(app), m_networkManager(app.getNetworkManager()),
                                             m_serverIp(0,0,0,0){
    DEBUG_CLOG(this, "Please enter Ip and Port:");

    std::string ipInput;
    std::cin >> ipInput >> m_serverPort;
    m_serverIp = *m_serverIp.resolve(ipInput);

    DEBUG_CLOG(this, "Trying to connect to" + m_serverIp.toString());
    m_networkManager.connect(m_serverIp, m_serverPort);
}

ClientState::~ClientState() {
    m_networkManager.disconnect();
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

}

void ClientState::inputProcess(float dt) {

}
