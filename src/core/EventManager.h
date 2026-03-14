//
// Created by user on 3/13/2026.
//

#ifndef YUKATARI_EVENTMANAGER_H
#define YUKATARI_EVENTMANAGER_H
#include <vector>

#include "IObserver.h"


class EventManager {
    std::vector<IObserver*> m_observers;
public:
    virtual ~EventManager() = default;

    void addObserver(IObserver &observer);
    void removeObserver(IObserver &observer);

    void publish(IEvent& event);
};


#endif //YUKATARI_EVENTMANAGER_H