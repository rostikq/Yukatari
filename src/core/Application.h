//
// Created by user on 2/24/2026.
//

#ifndef YUKATARI_APPLICATION_H
#define YUKATARI_APPLICATION_H

#include  <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

#include "State.h"
#include "Timer.h"

#pragma pack(1)

class Application {
    bool m_initSuccess;
    bool m_isRunning;

    Timer m_timer;


    float m_lastDeltaTime = 0;
    int m_maxFramerate;

    sf::RenderWindow* m_windowPtr;
    State* m_statePtr;

    void render();

    void run();

    Application(const Application& other) = delete;
    Application(Application&& other) = delete;
public:
    Application(unsigned short width, unsigned short height, const char* title, int maxFramerate = 60);
    ~Application();

    void start();
};


#endif //YUKATARI_APPLICATION_H