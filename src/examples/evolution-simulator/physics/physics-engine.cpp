//
// Created by Артем on 04.02.2022.
//

#include "physics-engine.hpp"
#include "../../../utils/vec2.hpp"

void PhysicsEngine::tick(float dt) {
    clear_forces();

    for(auto& spring : physics_springs) {
        spring->force_tick(dt);
    }

    for(auto& vertex : physics_vertices) {
        vertex->m_force += m_gravity * vertex->m_mass;
    }

    apply_forces(dt);
    apply_velocities(dt);
}

void PhysicsEngine::clear_forces() {
    for(auto& vertex : physics_vertices) {
        vertex->m_force = {0, 0, 0};
    }
}

void PhysicsEngine::apply_forces(float dt) {
    for(auto& vertex : physics_vertices) {
        if(vertex->m_position.y < m_floor_level) {
            vertex->m_position.y = m_floor_level;

            if(vertex->m_velocity.y < 0) vertex->m_velocity.y = 0;

            if(vertex->m_force.y < 0) {
                float friction = -vertex->m_force.y * vertex->m_floor_friction;
                float epsilon = friction * dt / vertex->m_mass;

                Vec2f horizontal_velocity = { vertex->m_velocity.x, vertex->m_velocity.z };
                Vec2f horizontal_force = { vertex->m_force.x, vertex->m_force.z };
                float velocity_modulo = horizontal_velocity.len();

                if(velocity_modulo < epsilon) {
                    horizontal_velocity = {0, 0};
                    horizontal_force = {0, 0};
                }
                else horizontal_force -= (horizontal_velocity / velocity_modulo) * friction;

                vertex->m_velocity.x = horizontal_velocity.x;
                vertex->m_velocity.z = horizontal_velocity.y;

                vertex->m_force.x = horizontal_force.x;
                vertex->m_force.z = horizontal_force.y;
            }
        }

        vertex->m_velocity += vertex->m_force * dt / vertex->m_mass;
    }
}

void PhysicsEngine::apply_velocities(float dt) {
    for(auto& vertex : physics_vertices) {
        vertex->m_position += vertex->m_velocity * dt;
    }
}
