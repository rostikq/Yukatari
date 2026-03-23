//
// Created by user on 3/23/2026.
//

#ifndef YUKATARI_PLAYER_H
#define YUKATARI_PLAYER_H
#include "Entity.h"
#include "IDynamic.h"
#include "../InputInfo.h"


class Player : public Entity {
    InputInfo m_lastInputInfo;
    float m_speed = 5;
public:
    Player(World& world, sf::Vector3f pos = {0,0, 0}, float speed = 3.0f);

    void update(float dt) override;

    void setInput(InputInfo inputInfo) {m_lastInputInfo = inputInfo;}
};


#endif //YUKATARI_PLAYER_H