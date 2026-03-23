//
// Created by user on 3/17/2026.
//

#ifndef YUKATARI_ENTITY_H
#define YUKATARI_ENTITY_H


#include <vector>

#include "IDynamic.h"
#include "../../../core/EventManager.h"
#include "SFML/Network/Packet.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/System/Vector3.hpp"

class World;

struct EntitySnapshot {
    size_t id = 0;
    sf::Vector3f position {0,0,0};
    float rotation = 0;
};

inline sf::Packet& operator<<(sf::Packet &packet, const EntitySnapshot& snap) {
    packet << snap.id <<
        snap.position.x <<
            snap.position.y <<
                snap.position.z <<
            snap.rotation;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, EntitySnapshot snap) {
    packet >> snap.id;
    packet >> snap.position.x >> snap.position.y >> snap.position.z >> snap.rotation;
    return packet;
}

#define FLAG_DIRTY_ENT_TRANSFORM (1 << 0)

class Entity : public IDynamic, public EventManager {
protected:
    sf::Vector3f m_position;
    float m_rotation;
    sf::Vector2f m_size;

    sf::Vector3f m_velocity;

    float m_dragXY;
    float m_dragZ;

    uint16_t m_dirtyFlags = 0;



    World& m_world;

    std::vector<Entity*> m_children;
public:
    Entity(World& world ,sf::Vector3f pos = {0,0, 0}, float rotation = 0);

    size_t id = 0;

    virtual ~Entity() = default;

    sf::Vector3f getPosition() const { return m_position;}
    float getRotation() const { return m_rotation;}

    void setPosition(sf::Vector3f pos);

    void setRotation(float rotation);

    void move(sf::Vector3f offset);
    void rotate(float rotation);

    EntitySnapshot getSnapshot() {
        EntitySnapshot snap{
        id, m_position, m_rotation};
        return snap;
    }

    void update(float dt) override;
};


#endif //YUKATARI_ENTITY_H