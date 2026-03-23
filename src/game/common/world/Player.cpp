//
// Created by user on 3/23/2026.
//

#include "Player.h"

#include "../../../Debug.h"

Player::Player(World &world, sf::Vector3f pos, float speed):
Entity(world, pos), m_lastInputInfo(), m_speed(speed){
    m_dragXY = 0.9f;
}

void Player::update(float dt) {
    if (m_lastInputInfo.x != 0 || m_lastInputInfo.y != 0) {
        sf::Vector3f direction = sf::Vector3f(m_lastInputInfo.x, m_lastInputInfo.y, 0);

        if (direction.lengthSquared() > 1) {
            direction = direction.normalized();
        }
        m_velocity = direction * m_speed;
    }

    Entity::update(dt);

}
