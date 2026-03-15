//
// Created by user on 2/24/2026.
//

#ifndef YUKATARI_APPLICATION_H
#define YUKATARI_APPLICATION_H

#include  <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

#include "IEvent.h"
#include "NetworkManager.h"
#include "Timer.h"


class State;

class Application : public EventManager {
    bool m_initSuccess;
    bool m_isRunning;

    Timer m_timer;

    float m_lastDeltaTime = 0;
    int m_maxFramerate;

    sf::RenderWindow* m_windowPtr = nullptr;
    State* m_statePtr = nullptr;
    NetworkManager m_networkManager;

    void test_eventCallback(float yo);

    void render();

    void run();

    Application(const Application& other) = delete;
    Application(Application&& other) = delete;
public:
    Application(unsigned short width, unsigned short height, const char* title, int maxFramerate = 60);
    ~Application();

    void start();
    void setState(State* state) {
        m_statePtr = state;
    }

    NetworkManager& getNetworkManager(){ return m_networkManager; }
    float getDeltaTime() const { return m_lastDeltaTime; }
    sf::RenderWindow* getWindowPtr() {return m_windowPtr;}
};


#endif //YUKATARI_APPLICATION_H