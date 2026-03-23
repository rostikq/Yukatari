//
// Created by user on 3/17/2026.
//

#include "Entity.h"

#include <iostream>

#include "../../client/events/EventWorldLoad.h"
#include "../../server/events/EventEntityTransformUpdate.h"

Entity::Entity(World &world, sf::Vector3f pos, float rotation) :
m_world(world),
m_position(pos),
m_rotation(rotation),
m_velocity(0,0,0){
    m_dragXY = 0.1f;
}

void Entity::setPosition(sf::Vector3f pos) {
    if (pos == m_position) return;
    m_position = pos;
    m_dirtyFlags |= FLAG_DIRTY_ENT_TRANSFORM;
}

void Entity::setRotation(float rotation) {
    m_rotation = rotation;
    m_dirtyFlags |= FLAG_DIRTY_ENT_TRANSFORM;
}

void Entity::move(sf::Vector3f offset) {
    if (offset.lengthSquared() == 0) return;
    m_position += offset;
    m_dirtyFlags |= FLAG_DIRTY_ENT_TRANSFORM;
}

void Entity::rotate(float rotation) {
    m_rotation += rotation;
    m_dirtyFlags |= FLAG_DIRTY_ENT_TRANSFORM;
}

void Entity::update(float dt) {
    m_velocity.x = m_velocity.x *  (1-m_dragXY * dt);
    m_velocity.y = m_velocity.y * (1-m_dragXY* dt);
    m_velocity.z = m_velocity.z * (1-m_dragZ* dt);

    if (m_velocity.lengthSquared() < 1e-3) m_velocity = sf::Vector3f(0,0,0);

    move(m_velocity * dt);

    if ((m_dirtyFlags & FLAG_DIRTY_ENT_TRANSFORM) != 0) {
        EventEntityTransformUpdate event;
        std::cout << id << " has dirty transform update";
        event.id = id;
        event.posX = m_position.x;
        event.posY = m_position.y;
        event.posZ = m_position.z;
        event.rotation = m_rotation;
        publish(event);
    }
    m_dirtyFlags = 0;
}
