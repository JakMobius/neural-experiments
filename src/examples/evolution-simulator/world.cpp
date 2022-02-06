//
// Created by Артем on 04.02.2022.
//

#include "world.hpp"
#include "../../graphics/scene-rendering/shape-generator.hpp"

void EvolutionWorld::tick(float dt) {
    for(auto& object : m_objects) {
        object->tick(dt);
    }
}

void EvolutionWorld::add_floor() {
    ShapeGenerator generator;

    generator.add_cube({0, -0.05, 0}, {1000, 0.1, 1000}, {0.15, 0.5, 0.2});
    m_floor = m_renderer->get_geometry_pool()->create_object({generator.get_mesh()}, nullptr);
    generator.reset();
}
