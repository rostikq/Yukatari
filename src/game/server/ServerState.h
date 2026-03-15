//
// Created by user on 3/14/2026.
//

#ifndef YUKATARI_SERVERSTATE_H
#define YUKATARI_SERVERSTATE_H
#include "Client.h"
#include "../../core/State.h"


class ServerState : public State{
private:
    sf::UdpSocket m_udpSocket;
    unsigned short m_port;
    bool m_isConnected;

    std::vector<Client> m_clients;
public:
    ServerState(Application& application);
    ~ServerState();

    void update(float dt) override;
    void inputProcess(float dt) override;
    void render(float dt) override;
};


#endif //YUKATARI_SERVERSTATE_H