//
// Created by user on 3/15/2026.
//

#ifndef YUKATARI_CAMERA_H
#define YUKATARI_CAMERA_H
#include "SFML/System/Vector2.hpp"
#include "world/Entity.h"
#include "world/IDynamic.h"


class Camera : public IDynamic {
    sf::Vector2f m_position;
    float m_scale;

    Entity* m_followEntity = nullptr;
public:
    Camera(sf::Vector2f position = {0,0}, float scale = 1) : m_position(position), m_scale(scale) {}
    sf::Vector2f getPosition() const { return m_position; }
    float getScale() const { return m_scale; }

    void setPosition(sf::Vector2f position) { m_position = position; }
    void move(sf::Vector2f direction) { m_position += direction; }
    void setScale(float scale) { m_scale = scale; }

    void update(float dt) override;

    void setFollowEntity(Entity* entity) { m_followEntity = entity; }
};


#endif //YUKATARI_CAMERA_H