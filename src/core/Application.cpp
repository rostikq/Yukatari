//
// Created by user on 2/24/2026.
//

#include "Application.h"
#include "State.h"

#include <format>
#include <functional>
#include <iostream>
#include <thread>
#include <math.h>
#include "../Debug.h"

Application::Application(unsigned short width, unsigned short height, const char *title, int maxFramerate)
    : m_timer(), m_maxFramerate(maxFramerate), m_networkManager() {
    DEBUG_CLOG(this, "Initializing application...");

    m_windowPtr = new sf::RenderWindow(sf::VideoMode({width, height}), title);
    if (m_windowPtr == nullptr) {
        DEBUG_CLOG(this, "Window creation failed");
        return;
    }
    DEBUG_CLOG(this, "Window creation successfull");
    m_windowPtr->setFramerateLimit(m_maxFramerate);

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

void Application::render() {
    DEBUG_CLOG(this, "Render thread created");
    m_windowPtr->setActive(true);

    while (m_windowPtr->isOpen() && m_isRunning) {
        m_windowPtr->clear();
        if (m_statePtr) m_statePtr->render(m_lastDeltaTime);
        m_windowPtr->display();
    }
}

void Application::run() {
    m_isRunning = true;

    m_windowPtr->setActive(false);
    std::thread renderThread(&Application::render, this);

    while (m_windowPtr->isOpen() && m_isRunning) {
        m_timer.reset();
        while (const std::optional<sf::Event> event = m_windowPtr->pollEvent()) {
            if (event->is<sf::Event::Closed>()) m_windowPtr->close();
        }

        if (m_statePtr) {
            m_statePtr->inputProcess(m_lastDeltaTime);
            m_statePtr->update(m_lastDeltaTime);
        }

        m_lastDeltaTime = m_timer.getPassedTimeInSeconds();
    }

    renderThread.join();
}

void Application::test_eventCallback(float yo) {

}
