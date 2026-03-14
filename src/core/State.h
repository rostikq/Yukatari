//
// Created by user on 3/13/2026.
//

#ifndef YUKATARI_STATE_H
#define YUKATARI_STATE_H

#include "Application.h"

class State {
protected:
    Application& m_application;
public:
    State(Application& application) : m_application(application) {
    }
    virtual ~State() = default;

    virtual void inputProcess(float dt) = 0;
    virtual void update(float dt) = 0;
    virtual void render(float dt) = 0;
};


#endif //YUKATARI_STATE_H