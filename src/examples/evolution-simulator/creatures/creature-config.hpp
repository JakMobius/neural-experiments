#pragma once

#include <vector>
#include <random>
#include "../../../utils/vec3.hpp"
#include "../objects/vertex-object.hpp"
#include "../objects/spring-object.hpp"
#include "../objects/muscle.hpp"
#include "../../../perceptron/neural-network-config.hpp"

struct VertexConfig {
    float m_mass;
    float m_floor_friction;
    Vec3f m_position;

    void apply(VertexObject* vertex) const;
};

struct SpringConfig {
    float m_strength;
    float m_damping;
    int m_vertex_a_index;
    int m_vertex_b_index;

    void apply(SpringObject* spring) const;
};

struct MuscleConfig {
    int m_spring_index;
    float m_min_length_coefficient;
    float m_max_length_coefficient;

    void apply(Muscle* muscle, SpringObject* spring) const;
};

struct CreatureConfig {
    std::vector<VertexConfig> m_vertices;
    std::vector<SpringConfig> m_springs;
    std::vector<MuscleConfig> m_muscles;
    NeuralNetworkConfig m_brain_config;
};