//
// Created by user on 2/24/2026.
//

#include "Application.h"

#include <format>
#include <iostream>
#include <thread>

#include "../Debug.h"

Application::Application(unsigned short width, unsigned short height, const char *title, int maxFramerate) {
    DEBUG_CLOG(this, "Initializing application...");

    m_windowPtr = new sf::RenderWindow(sf::VideoMode({width, height}), title);
    if (m_windowPtr == nullptr) {
        DEBUG_CLOG(this, "Window creation failed");
        return;
    }
    DEBUG_CLOG(this, "Window creation successfull");

    m_initSuccess = true;
}

Application::~Application() {
    m_windowPtr->close();
    delete m_windowPtr;
    m_windowPtr = nullptr;
    DEBUG_CLOG(this, "Application destroyed");
}

void Application::start() {
    if (!m_initSuccess) return;
    run();
}

void Application::run() {
    m_isRunning = true;

    while (m_windowPtr->isOpen() && m_isRunning) {
        m_timer.reset();
        while (const std::optional<sf::Event> event = m_windowPtr->pollEvent()) {
            if (event->is<sf::Event::Closed>()) m_windowPtr->close();
        }

        m_windowPtr->clear(sf::Color::Cyan);
        m_windowPtr->display();

        while (1.0f/m_timer.getPassedTimeInSeconds() > m_maxFramerate) {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
        m_lastDeltaTime = m_timer.getPassedTimeInSeconds();
    }
}
