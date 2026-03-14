//
// Created by user on 3/14/2026.
//

#include "ServerState.h"

#include <iostream>ServerState::ServerState(Application &application) : State(application) {
    DEBUG_CLOG(this, "Please enter port for server:");
    std::cin >> m_port;
    if (m_udpSocket.bind(m_port) != sf::Socket::Status::Done) {
        DEBUG_CLOG(this, "Error")
    }
}


#include "../../Debug.h"

