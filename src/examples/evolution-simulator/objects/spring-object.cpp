//
// Created by Артем on 04.02.2022.
//

#include "spring-object.hpp"
#include "../world.hpp"
#include "../../../graphics/scene-rendering/shape-generator.hpp"

SpringObject::SpringObject(EvolutionWorld* world) : WorldObject(world) {
    m_physics_spring = std::make_unique<PhysicsSpring>();

    ShapeGenerator generator;
    generator.add_cube({}, {1.0, 0.2, 0.2}, {0, 1, 0});

    m_geometry_object = m_world->get_renderer()->get_geometry_pool()->create_object({ generator.get_mesh() }, nullptr);
    m_world->get_physics_engine()->register_spring(m_physics_spring.get());
    m_world->add_object(this);
}

SpringObject::~SpringObject() {
    m_world->get_renderer()->get_geometry_pool()->remove_object(m_geometry_object);
    m_world->get_physics_engine()->delete_spring(m_physics_spring.get());
    m_world->remove_object(this);
}

void SpringObject::tick(float dt) {
    Vec3f& a_position = m_physics_spring->m_vertex_a->m_position;
    Vec3f& b_position = m_physics_spring->m_vertex_b->m_position;

    Vec3f center = (a_position + b_position) *= 0.5;

    Vec3f basis_x = b_position - a_position;
    Vec3f basis_y = get_orthogonal(basis_x.normal());
    Vec3f basis_z = basis_x.normal().cross(basis_y);

    Matrix4f transform({
       basis_x.x, basis_x.y, basis_x.z, 0,
       basis_y.x, basis_y.y, basis_y.z, 0,
       basis_z.x, basis_z.y, basis_z.z, 0,
       center.x, center.y, center.z, 1,
   });

    m_geometry_object->set_transform(transform);
}

Vec3f SpringObject::get_orthogonal(const Vec3f &direction) {
    Vec3f up_a = {direction.y, -direction.x, 0};
    Vec3f up_b = {0, -direction.z, direction.x};

    float projection_a = 1 - abs(up_a.dot(direction));
    float projection_b = 1 - abs(up_b.dot(direction));

    Vec3f result = (up_a *= projection_a) += (up_b *= projection_b);
    result.normalize();

    return direction.cross(result);
}

