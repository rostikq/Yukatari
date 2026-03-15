//
// Created by user on 3/14/2026.
//

#ifndef YUKATARI_SERVERSTATE_H
#define YUKATARI_SERVERSTATE_H
#include "Client.h"
#include "../../core/State.h"
#include "../common/Camera.h"
#include "../common/World.h"
#include "../common/WorldRenderer.h"


class ServerState : public State{
private:
    sf::UdpSocket m_udpSocket;
    unsigned short m_port;
    bool m_isConnected;

    World m_world;
    WorldRenderer m_worldRenderer;
    Camera m_camera;
    sf::RenderWindow* m_window = nullptr;

    float m_scale = 1;
    float m_tileSizeX = 1;
    float m_tileSizeY = 0.5f;
    sf::ConvexShape m_tileShape;

    std::vector<Client> m_clients;


public:
    ServerState(Application& application);
    ~ServerState();

    void update(float dt) override;
    void inputProcess(float dt) override;
    void render(float dt) override;
};


#endif //YUKATARI_SERVERSTATE_H