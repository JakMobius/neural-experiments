#pragma once

#include "world-object.hpp"
#include "../physics/spring.hpp"
#include "../../../graphics/scene-rendering/geometry-object.hpp"

class SpringObject : public WorldObject {
    std::unique_ptr<PhysicsSpring> m_physics_spring {};
    Graphics::GeometryObject* m_geometry_object = nullptr;

    static Vec3f get_orthogonal(const Vec3f &direction);

public:
    explicit SpringObject(EvolutionWorld* world);
    ~SpringObject() override;

    PhysicsSpring* get_physics_spring() { return m_physics_spring.get(); }
    Graphics::GeometryObject* get_geometry_object() { return m_geometry_object; }

    void tick(float dt) override;
};