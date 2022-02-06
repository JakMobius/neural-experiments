//
// Created by Артем on 04.02.2022.
//

#include "vertex-object.hpp"
#include "../world.hpp"
#include "../../../graphics/scene-rendering/shape-generator.hpp"

VertexObject::VertexObject(EvolutionWorld* world) : WorldObject(world) {
    m_physics_vertex = std::make_unique<PhysicsVertex>();
    m_world->add_object(this);
}

VertexObject::~VertexObject() {
    if(m_geometry_object) {
        m_world->get_renderer()->get_geometry_pool()->remove_object(m_geometry_object);
    }
    m_world->remove_object(this);
}

void VertexObject::create_colored_mesh(const Vec3f& color) {
    delete m_geometry_object;

    ShapeGenerator generator;
    generator.add_cube({}, {0.5, 0.5, 0.5}, color);

    auto geometry_pool = m_world->get_renderer()->get_geometry_pool();
    m_geometry_object = geometry_pool->create_object({ generator.get_mesh() }, nullptr);
}
