//
// Created by user on 3/15/2026.
//

#ifndef YUKATARI_CLIENTSTATE_H
#define YUKATARI_CLIENTSTATE_H
#include "../../core/State.h"


class ClientState final: public State {
    NetworkManager& m_networkManager;
    sf::Window* m_window = nullptr;
    sf::IpAddress m_serverIp;
    unsigned short m_serverPort;
    float pingTime = 1.0f;
public:
    ClientState(Application& app);
    ~ClientState();


    void update(float dt) override;
    void inputProcess(float dt) override;
    void render(float dt) override;
};


#endif //YUKATARI_CLIENTSTATE_H