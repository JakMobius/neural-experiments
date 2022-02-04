#pragma once

#include "vertex.hpp"

struct PhysicsSpring {
    PhysicsVertex* m_vertex_a = nullptr;
    PhysicsVertex* m_vertex_b = nullptr;
    float m_target_length = 1;
    float m_strength = 1;
    float m_damping = 1;

    void force_tick(float dt) {
        Vec3f& a_position = m_vertex_a->m_position;
        Vec3f& b_position = m_vertex_b->m_position;

        Vec3f& a_velocity = m_vertex_a->m_velocity;
        Vec3f& b_velocity = m_vertex_b->m_velocity;

        Vec3f relative_velocity = b_velocity - a_velocity;

        Vec3f distance_vector = b_position;
        distance_vector -= a_position;

        float spring_length = distance_vector.len();
        Vec3f distance_normalized = distance_vector / spring_length;

        float relative_speed = relative_velocity.dot(distance_normalized);
        float force = (m_target_length / spring_length - 1) * m_strength - relative_speed * m_damping;

        Vec3f force_vector = distance_normalized * force;

        m_vertex_a->m_force -= force_vector;
        m_vertex_b->m_force += force_vector;
    }
};