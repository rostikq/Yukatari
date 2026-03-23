//
// Created by user on 3/15/2026.
//

#ifndef YUKATARI_CLIENTSTATE_H
#define YUKATARI_CLIENTSTATE_H
#include "../../core/State.h"
#include "../common/Camera.h"
#include "../common/World.h"
#include "../common/WorldRenderer.h"


class ClientState final: public State, public IObserver {
    NetworkManager& m_networkManager;
    sf::RenderWindow* m_window = nullptr;
    sf::IpAddress m_serverIp;
    unsigned short m_serverPort;

    World m_world;
    WorldRenderer m_worldRenderer;

    Camera m_camera;
    InputInfo m_lastInputInfo;

    Entity* m_playerEntity = nullptr;

    float pingTime = 1.0f;
public:
    ClientState(Application& app);
    ~ClientState();


    void update(float dt) override;
    void inputProcess(float dt) override;
    void render(float dt) override;

    void terminate();

    void onEvent(IEvent &event) override;
};


#endif //YUKATARI_CLIENTSTATE_H