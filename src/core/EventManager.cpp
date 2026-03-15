//
// Created by user on 3/13/2026.
//

#include "EventManager.h"

void EventManager::addObserver(IObserver &observer) {
        m_observers.push_back(&observer);
}
void EventManager::removeObserver(IObserver &observer) {
        for (auto it = m_observers.begin(); it != m_observers.end(); ++it) {
                if (*it == &observer) {
                        m_observers.erase(it);
                        return;
                }
        }
}

void EventManager::publish(IEvent &event) {
        if (m_observers.empty()) return;
        for (auto &observer : m_observers) {
                if (observer == nullptr) continue;
                observer->onEvent(event);
        }
}
