#pragma once

#include <vector>
#include <unordered_set>
#include "vertex.hpp"
#include "spring.hpp"

class PhysicsEngine {
    std::unordered_set<PhysicsVertex*> physics_vertices {};
    std::unordered_set<PhysicsSpring*> physics_springs {};

    Vec3f m_gravity = { 0, -9.81, 0 };
    float m_floor_level = 0.25;

    void clear_forces();
    void apply_forces(float dt);
    void apply_velocities(float dt);

public:
    PhysicsEngine() {}

    void register_vertex(PhysicsVertex* vertex) { physics_vertices.insert(vertex); }
    void register_spring(PhysicsSpring* spring) { physics_springs.insert(spring); }
    void delete_vertex(PhysicsVertex* vertex) { physics_vertices.erase(vertex); }
    void delete_spring(PhysicsSpring* spring) { physics_springs.erase(spring); }

    void tick(float dt);
};