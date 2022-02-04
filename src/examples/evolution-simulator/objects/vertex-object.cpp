//
// Created by Артем on 04.02.2022.
//

#include "vertex-object.hpp"
#include "../world.hpp"
#include "../../../graphics/scene-rendering/shape-generator.hpp"

VertexObject::VertexObject(EvolutionWorld* world) : WorldObject(world) {
    m_physics_vertex = std::make_unique<PhysicsVertex>();

    ShapeGenerator generator;
    generator.add_cube({}, {0.5, 0.5, 0.5}, {1, 0, 0});

    m_geometry_object = m_world->get_renderer()->get_geometry_pool()->create_object({ generator.get_mesh() }, nullptr);
    m_world->get_physics_engine()->register_vertex(m_physics_vertex.get());
    m_world->add_object(this);
}

VertexObject::~VertexObject() {
    m_world->get_renderer()->get_geometry_pool()->remove_object(m_geometry_object);
    m_world->get_physics_engine()->delete_vertex(m_physics_vertex.get());
    m_world->remove_object(this);
}
