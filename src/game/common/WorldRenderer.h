//
// Created by user on 3/15/2026.
//

#ifndef YUKATARI_WORLDRENDERER_H
#define YUKATARI_WORLDRENDERER_H
#include "Camera.h"
#include "World.h"
#include "SFML/Graphics/ConvexShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"


class WorldRenderer {
private:
    sf::ConvexShape m_tileShape;

    float m_scale = 1;
public:
    WorldRenderer();
    ~WorldRenderer() = default;
    void draw(sf::RenderWindow* window ,World& world, Camera& camera);
};


#endif //YUKATARI_WORLDRENDERER_H