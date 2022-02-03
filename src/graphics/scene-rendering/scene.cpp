//
// Created by Артем on 31.01.2022.
//

#include "scene.hpp"
#include "shadow-map-framebuffer.hpp"
#include "shape-generator.hpp"

namespace Graphics {

Scene::Scene() {
    m_renderer = std::make_unique<SceneRenderer>(this);
}

void Scene::draw() {
    m_renderer->draw();
}

void Scene::set_screen_size(const Vec2i &screen_size) { m_renderer->set_screen_size(screen_size); }

void Scene::set_camera(Camera* camera) { m_renderer->set_camera(camera); }

}