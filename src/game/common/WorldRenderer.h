//
// Created by user on 3/15/2026.
//

#ifndef YUKATARI_WORLDRENDERER_H
#define YUKATARI_WORLDRENDERER_H
#include "Camera.h"
#include "World.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/ConvexShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"


class WorldRenderer {
private:
    sf::ConvexShape m_tileShape;
    sf::RectangleShape m_circleShape;

    float m_lod_threshold = 20.0f;
    float m_scale = 1;
public:
    WorldRenderer();
    ~WorldRenderer() = default;

    sf::Vector2f projectOnDisplay(sf::Vector3f point);
    void draw(sf::RenderWindow* window ,World& world, Camera& camera);
};


#endif //YUKATARI_WORLDRENDERER_H