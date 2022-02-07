#pragma once

#include <random>
#include <vector>
#include <set>
#include "creature-config.hpp"

struct CreatureGenerator {
    int m_max_vertices = 6;
    int m_min_vertices = 4;

    float m_max_mass = 0.2;
    float m_min_mass = 0.2;

    float m_min_friction = 0.1;
    float m_max_friction = 2.0;

    Vec3f m_min_pos = {-0.5, 0.5, -0.5};
    Vec3f m_max_pos = { 0.5, 1, 0.5};

    float m_min_damping = 0.15;
    float m_max_damping = 0.15;

    float m_min_strength = 25.0;
    float m_max_strength = 50.0;

    float m_min_muscles = 4;
    float m_max_muscles = 10;

    int m_max_layers = 1;
    int m_min_layers = 1;

    int m_min_brain_murations = 0;
    int m_max_brain_murations = 4;

    mutable std::mt19937 m_rnd {};

public:

    void generate_creature(CreatureConfig* target);
    void mutate_creature(CreatureConfig* target);

    template<typename T>
    void seed(T seed) { m_rnd.seed(seed); }
};