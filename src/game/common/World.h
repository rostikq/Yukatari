//
// Created by user on 3/15/2026.
//

#ifndef YUKATARI_WORLD_H
#define YUKATARI_WORLD_H
#include <format>
#include <iostream>
#include <map>
#include <vector>

#include "../../Debug.h"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/System/Vector2.hpp"
#include "world/Entity.h"
#include "world/IdGenerator.h"
#include "world/MapInfo.h"
#include "world/Player.h"
#include "world/Tile.h"


class World : public IObserver, public EventManager {
private:
    std::map<unsigned int, Entity*> m_entities;
    std::map<unsigned int, Player*> m_players;

    std::map<unsigned int, bool> m_updatedEntities;

    IdGenerator m_idGenerator;

    std::vector<sf::Vector2i> m_dirtyTiles;

    sf::VertexArray m_renderVertexArray;
public:
    World();
    ~World();

    size_t mapSizeX = 512;
    size_t mapSizeY = 512;
    TileType *map = nullptr;
    std::vector<sf::Vector2i> spawnTiles;

    MapInfo mapInfo;

    void loadMap(MapInfo &mapInfo);

    unsigned int addEntity(Entity *entity);
    void removeEntity(Entity *entity);
    void removeEntity(unsigned int id);
    Entity *getEntity(unsigned int id);

    unsigned int createPlayer();
    Player* getPlayer(unsigned int id) { return m_players.find(id)->second; }


    void update(float dt);

    std::map<unsigned int, Entity*>& getEntities() { return m_entities; }

    MapInfo getMapInfo();

    void updateRenderVertexArray();

    sf::VertexArray &getRenderVertexArray() { return m_renderVertexArray; }

    std::vector<EntitySnapshot> getEntitySnapshots() {
        std::vector<EntitySnapshot> snapshots;
        for (auto it = m_entities.begin(); it != m_entities.end(); ++it) {
            if ((*it).second != nullptr) {
                snapshots.push_back((*it).second->getSnapshot());
            }
        }
        DEBUG_CLOG(this, std::format("Took snapshot of {} entites", snapshots.size()));
        return snapshots;
    }

    size_t getEntitiesMapSize() { return m_entities.size(); }

    void loadEntities(const std::vector<EntitySnapshot>& snapshots);

    void onEvent(IEvent &event) override;

};


#endif //YUKATARI_WORLD_H