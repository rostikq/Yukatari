//
// Created by user on 3/14/2026.
//

#ifndef YUKATARI_SERVERSTATE_H
#define YUKATARI_SERVERSTATE_H
#include "../../core/State.h"


class ServerState : public State{
private:
    sf::UdpSocket m_udpSocket;
    unsigned short m_port;
public:
    ServerState(Application& application);
    ~ServerState();
};


#endif //YUKATARI_SERVERSTATE_H