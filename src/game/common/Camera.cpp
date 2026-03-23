//
// Created by user on 3/15/2026.
//

#include "Camera.h"

void Camera::update(float dt) {
    if (m_followEntity != nullptr) {
        sf::Vector3f entityPos = m_followEntity->getPosition();
        m_position = {entityPos.x, entityPos.y};
    }
}
