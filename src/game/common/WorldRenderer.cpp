//
// Created by user on 3/15/2026.
//

#include "WorldRenderer.h"

#include "resources/ResourceManager.h"

WorldRenderer::WorldRenderer() {
    m_tileShape.setPointCount(4);

    m_tileShape.setPoint(0, {-m_scale/2, 0});
    m_tileShape.setPoint(1, {0, -m_scale/4});
    m_tileShape.setPoint(2, {m_scale/2, 0});
    m_tileShape.setPoint(3, {0, m_scale/4});

    m_circleShape.setFillColor(sf::Color::Red);
    m_circleShape.setSize({1, 1});
}

void WorldRenderer::draw(sf::RenderWindow *window, World &world, Camera &camera) {
    if (!window) return;

    if (camera.getScale() != 0)
        m_scale = 1/camera.getScale();

    float centerX = window->getSize().x / 2.0f;
    float centerY = window->getSize().y / 2.0f;

    m_tileShape.setScale({m_scale, m_scale});

    sf::VertexArray& vertexArray = world.getRenderVertexArray();
    sf::RenderStates renderStates;

    sf::Vector2f camPos = camera.getPosition();

    float cameraX = (camPos.x - camPos.y) / 2;
    float cameraY = (camPos.x + camPos.y) / 4;

    renderStates.transform.translate({centerX, centerY});
    renderStates.transform.scale({m_scale, m_scale});
    renderStates.transform.translate({-cameraX, -cameraY});


    if (m_scale < m_lod_threshold)
    renderStates.texture = ResourceManager::getInstance().getTexture(2);
    else {
        renderStates.texture = ResourceManager::getInstance().getTexture(1);
    }

    window->draw(vertexArray, renderStates);

    auto& entities = world.getEntities();

    if (!entities.empty()) {
        for (auto it = entities.begin(); it != entities.end(); ++it) {
            if ((*it).second == nullptr) return;
            Entity* entity = (*it).second;
            sf::Vector3f point = entity->getPosition();
            point.x -= camera.getPosition().x;
            point.y -= camera.getPosition().y;


            float projectedX = (point.x - point.y) * m_scale / 2.0f;
            float projectedY = (point.x + point.y) * m_scale / 4.0f;

            projectedX += centerX;
            projectedY += centerY;

            m_circleShape.setPosition({projectedX, projectedY});
            m_circleShape.setScale({m_scale, m_scale});

            window->draw(m_circleShape);
        }
    }
}

sf::Vector2f WorldRenderer::projectOnDisplay(sf::Vector3f point) {

}
