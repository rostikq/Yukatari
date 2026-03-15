//
// Created by user on 3/15/2026.
//

#include "WorldRenderer.h"

WorldRenderer::WorldRenderer() {
    m_tileShape.setPointCount(4);

    m_tileShape.setPoint(0, {-m_scale/2, 0});
    m_tileShape.setPoint(1, {0, -m_scale/4});
    m_tileShape.setPoint(2, {m_scale/2, 0});
    m_tileShape.setPoint(3, {0, m_scale/4});
}

void WorldRenderer::draw(sf::RenderWindow *window, World &world, Camera &camera) {
    if (!window) return;

    if (camera.getScale() != 0)
        m_scale = 1/camera.getScale();

    float centerX = window->getSize().x / 2.0f;
    float centerY = window->getSize().y / 2.0f;

    m_tileShape.setScale({m_scale, m_scale});

    for (int wrldX=0; wrldX<world.mapSizeX;++wrldX) {
        for (int wrldY=0; wrldY<world.mapSizeY;++wrldY) {
            float aposX = wrldX - camera.getPosition().x;
            float aposY = wrldY - camera.getPosition().y;

            float projectedX = (aposX - aposY) * m_scale / 2.0f;
            float projectedY = (aposX + aposY) * m_scale / 4.0f;

            projectedX += centerX;
            projectedY += centerY;

            if (projectedX < 0 || projectedX > window->getSize().x || projectedY < 0 || projectedY > window->getSize().y)
                continue;
            m_tileShape.setPosition({projectedX, projectedY});

            sf::Color color;
            switch (world.map[wrldY * world.mapSizeY + wrldX]) {
                case EMPTY:
                    color = sf::Color::White;
                    break;
                case GRASS:
                    color = sf::Color::Green;
                    break;
                case WATER:
                    color = sf::Color::Blue;
                    break;
                case SAND:
                    color = sf::Color::Yellow;
                    break;
                default:
                    color = sf::Color::White;
                    break;
            }
            m_tileShape.setFillColor(color);

            window->draw(m_tileShape);

        }
    }
}
