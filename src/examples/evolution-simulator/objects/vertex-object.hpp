#pragma once

#include "world-object.hpp"
#include "../physics/vertex.hpp"
#include "../../../graphics/scene-rendering/geometry-object.hpp"

class VertexObject : public WorldObject {

    std::unique_ptr<PhysicsVertex> m_physics_vertex = nullptr;
    Graphics::GeometryObject* m_geometry_object = nullptr;

public:
    explicit VertexObject(EvolutionWorld* world);
    ~VertexObject();

    PhysicsVertex* get_physics_vertex() { return m_physics_vertex.get(); }
    Graphics::GeometryObject* get_geometry_object() { return m_geometry_object; }

    void tick(float dt) override {
        Vec3f& position = m_physics_vertex->m_position;
        m_geometry_object->set_transform(Matrix4f::translation_matrix(position.x, position.y, position.z));
    }
};