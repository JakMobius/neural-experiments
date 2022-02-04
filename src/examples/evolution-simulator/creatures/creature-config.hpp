#pragma once

#include <vector>
#include <random>
#include "../../../utils/vec3.hpp"
#include "../objects/vertex-object.hpp"
#include "../objects/spring-object.hpp"

struct VertexConfig {
    float m_mass;
    float m_floor_friction;
    Vec3f m_position;

    void apply(VertexObject* vertex) const;
};

struct SpringConfig {
    float m_strength;
    float m_damping;
    float m_target_length;
    int m_vertex_a_index;
    int m_vertex_b_index;

    void apply(SpringObject* spring) const;
};

struct CreatureConfig {
    std::vector<VertexConfig> m_vertices;
    std::vector<SpringConfig> m_springs;

    static CreatureConfig random(int seed) {
        std::mt19937 rnd(seed);


    }
};