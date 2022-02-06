#pragma once

#include <random>
#include <set>
#include "creature-config.hpp"
#include "creature-generator.hpp"

struct CreatureCreatingContext {
    CreatureConfig* m_result;
    std::vector<int> m_vertices_connections;
    std::vector<bool> m_spring_is_muscle;
    const CreatureGenerator* m_generator;
    std::set<int> m_variants;

    int m_vertex_count = 0;
    int m_spring_count = 0;
    int m_muscle_count = 0;
    int m_brain_layer_count = 0;
public:

    CreatureCreatingContext(const CreatureGenerator* generator, CreatureConfig* config):
        m_generator(generator), m_result(config) {
    }

    float random_float(float min = 0, float max = 1) const;
    double random_double(double min = 0, double max = 1) const;
    int random_int(int min = INT_MIN, int max = INT_MAX) const;
    Vec3f random_vector(const Vec3f &min, const Vec3f &max) const;

    VertexConfig random_vertex();
    SpringConfig random_spring();
    MuscleConfig random_muscle();

    int random_muscle_index();
    void tension_spring(SpringConfig& spring, int v1, int v2);

    bool tension_spring_randomly(SpringConfig& spring);
    bool tension_spring_randomly(SpringConfig& spring, int v1);

    void generate_creature();
    void generate_vertices();
    void generate_springs();
    void generate_muscles();
    void generate_brain();
};