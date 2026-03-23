//
// Created by user on 3/15/2026.
//

#include "World.h"

#include <complex>
#include <format>
#include <iostream>

#include "../../PerlinNoise.hpp"
#include "../../Debug.h"
#include "../server/events/EventEntityTransformUpdate.h"

#define MAP_SEED 696969696969;

World::World() {
    map = new TileType[mapSizeY * mapSizeX];
    DEBUG_CLOG(this, std::format("{} bytes allocated for map", mapSizeY * mapSizeX));

    const siv::PerlinNoise::seed_type seed = MAP_SEED;

    const siv::PerlinNoise perlin {seed};

    float grassHeight = 0.65;
    float sandHeight = 0.6;

    sf::Vector2f tileOffset = {32, 0};

    m_renderVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_renderVertexArray.resize(mapSizeX * mapSizeY * 6);

    size_t spawnCounter = 0;

    for (int y = 0; y < mapSizeY; y++) {
        for (int x = 0; x < mapSizeX; x++) {

            sf::Vertex* triangles = &m_renderVertexArray[(y * mapSizeX + x) * 6];

            sf::Vector2f offset;
            offset.x = (float)(x-y) / 2;
            offset.y = (float)(x+y) / 4;

            triangles[0].position = offset + sf::Vector2f((float)-1/2, 0);
            triangles[1].position = offset + sf::Vector2f(0, (float)-1/4);
            triangles[2].position = offset + sf::Vector2f((float)1/2, 0);
            triangles[3].position = offset + sf::Vector2f((float)-1/2, 0);
            triangles[4].position = offset + sf::Vector2f(0, (float)1/4);
            triangles[5].position = offset + sf::Vector2f((float)1/2, 0);

            float height = perlin.octave2D_01((x * 0.01), (y* 0.01), 4);
            if (height > grassHeight ) {
                map[mapSizeX * y + x] = TileType::GRASS;
                tileOffset = {32, 0};
                triangles[0].texCoords =tileOffset + sf::Vector2f(0,0);
                triangles[1].texCoords =tileOffset + sf::Vector2f(32,0);
                triangles[2].texCoords =tileOffset + sf::Vector2f(32,32);
                triangles[3].texCoords =tileOffset + sf::Vector2f(0,0);
                triangles[4].texCoords =tileOffset + sf::Vector2f(0,32);
                triangles[5].texCoords =tileOffset + sf::Vector2f(32,32);
                spawnCounter++;
            }
            else if (height > sandHeight) {
                map[mapSizeX * y + x] = TileType::SAND;
                tileOffset = {64, 0};
                triangles[0].texCoords =tileOffset + sf::Vector2f(0,0);
                triangles[1].texCoords =tileOffset + sf::Vector2f(32,0);
                triangles[2].texCoords =tileOffset + sf::Vector2f(32,32);
                triangles[3].texCoords =tileOffset + sf::Vector2f(0,0);
                triangles[4].texCoords =tileOffset + sf::Vector2f(0,32);
                triangles[5].texCoords =tileOffset + sf::Vector2f(32,32);
                spawnCounter++;
            }
            else {
                map[mapSizeX * y + x] = TileType::WATER;
                tileOffset = {96, 0};
                triangles[0].texCoords =tileOffset + sf::Vector2f(0,0);
                triangles[1].texCoords =tileOffset + sf::Vector2f(32,0);
                triangles[2].texCoords =tileOffset + sf::Vector2f(32,32);
                triangles[3].texCoords =tileOffset + sf::Vector2f(0,0);
                triangles[4].texCoords =tileOffset + sf::Vector2f(0,32);
                triangles[5].texCoords =tileOffset + sf::Vector2f(32,32);
            }

            if (spawnCounter == 100) {
                spawnCounter = 0;
                spawnTiles.emplace_back(x, y);
            }

        }
    }

    mapInfo.mapSizeX = mapSizeX;
    mapInfo.mapSizeY = mapSizeY;
    for (size_t i = 0; i < mapSizeY * mapSizeX; i++) {
        mapInfo.info.push_back((uint8_t)map[i]);
    }
}

MapInfo World::getMapInfo() {
    mapInfo.info.resize(mapSizeY * mapSizeX);
    for (size_t i = 0; i < mapSizeY * mapSizeX; i++) {
        mapInfo.info.at(i) = (uint8_t)map[i];
    }
    return mapInfo;
}

World::~World() {
    for (auto& entityPtr : m_entities) {
        removeEntity(entityPtr.first);
    }

    delete map;
}

void World::loadMap(MapInfo &mapInfo) {
    mapSizeY = mapInfo.mapSizeY;
    mapSizeX = mapInfo.mapSizeX;
    delete[] map;
    map = new TileType[mapSizeY * mapSizeX];
    for (size_t i = 0; i < mapSizeY * mapSizeX; i++) {
        map[i] = (TileType)mapInfo.info[i];
    }

    updateRenderVertexArray();
}

unsigned int World::addEntity(Entity *entity) {
    size_t id = m_idGenerator.allocateId();
    m_entities [id] = entity;
    entity->addObserver(*this);
    DEBUG_CLOG(this, std::format("New entity added to world with {} id", id) );
    return id;
}

void World::removeEntity(Entity *entity) {
    for (size_t i = 0; i < m_entities.size(); i++) {
        if (m_entities.at(i) == entity) {
            removeEntity(i);
            return;
        }
    }
}

    void World::updateRenderVertexArray() {
    m_renderVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_renderVertexArray.resize(mapSizeX * mapSizeY * 6);

    for (int y = 0; y < mapSizeY; y++) {
        for (int x = 0; x < mapSizeX; x++) {

            sf::Vertex* triangles = &m_renderVertexArray[(y * mapSizeX + x) * 6];

            sf::Vector2f offset;
            offset.x = (float)(x-y) / 2;
            offset.y = (float)(x+y) / 4;

            triangles[0].position = offset + sf::Vector2f((float)-1/2, 0);
            triangles[1].position = offset + sf::Vector2f(0, (float)-1/4);
            triangles[2].position = offset + sf::Vector2f((float)1/2, 0);
            triangles[3].position = offset + sf::Vector2f((float)-1/2, 0);
            triangles[4].position = offset + sf::Vector2f(0, (float)1/4);
            triangles[5].position = offset + sf::Vector2f((float)1/2, 0);

            TileType type = map[mapSizeX * y + x];
            if (type == TileType::GRASS ) {
                triangles[0].color = sf::Color::Green;
                triangles[1].color = sf::Color::Green;
                triangles[2].color = sf::Color::Green;
                triangles[3].color = sf::Color::Green;
                triangles[4].color = sf::Color::Green;
                triangles[5].color = sf::Color::Green;
            }
            else if (type == TileType::SAND) {
                map[mapSizeX * y + x] = TileType::SAND;
                triangles[0].color = sf::Color::Yellow;
                triangles[1].color = sf::Color::Yellow;
                triangles[2].color = sf::Color::Yellow;
                triangles[3].color = sf::Color::Yellow;
                triangles[4].color = sf::Color::Yellow;
                triangles[5].color = sf::Color::Yellow;
            }
            else {
                map[mapSizeX * y + x] = TileType::WATER;
                triangles[0].color = sf::Color::Blue;
                triangles[1].color = sf::Color::Blue;
                triangles[2].color = sf::Color::Blue;
                triangles[3].color = sf::Color::Blue;
                triangles[4].color = sf::Color::Blue;
                triangles[5].color = sf::Color::Blue;
            }
        }
        }
}

Entity *World::getEntity(unsigned int id) {
    return m_entities.at(id);
}

void World::removeEntity(unsigned int id) {
    if (m_entities.at(id) == nullptr) {
        DEBUG_CLOG(this, std::format("Attempt to delete unexisting entity {}", id));
        return;
    }
    delete m_entities.at(id);
    m_idGenerator.returnId(id);
    m_entities.at(id) = nullptr;
    DEBUG_CLOG(this, std::format("Entity {} deleted successfully", id));
}

void World::loadEntities(const std::vector<EntitySnapshot> &snapshots) {
    m_entities.clear();
    if (snapshots.empty()) return;
    for (auto& snapshot : snapshots) {
        Entity* entity = new Entity(*this);
        entity->id = snapshot.id;
        entity->setPosition(snapshot.position);
        entity->setRotation(snapshot.rotation);
        m_entities[snapshot.id] = entity;
    }
}

void World::update(float dt) {
    m_updatedEntities.clear();

    for (auto& player: m_players) {
        if (player.second != nullptr) {
            player.second->update(dt);
            m_updatedEntities[player.first] = true;
        }
    }
    for (auto& entity : m_entities) {
        if (entity.second != nullptr && m_updatedEntities.find(entity.first) == m_updatedEntities.end()) {
            entity.second->update(dt);
            m_updatedEntities[entity.first] = true;
        }
    }
}

unsigned int World::createPlayer() {
    auto* player = new Player(*this);

    auto id = addEntity(player);
    m_players[id] = player;
    return id;
}

void World::onEvent(IEvent &event) {
    if (auto* transformUpdateEvent = dynamic_cast<EventEntityTransformUpdate*>(&event)) {
        publish(*transformUpdateEvent);
    }
}
