#pragma once

#include <random>
#include <set>
#include "creature-config.hpp"
#include "creature-generator.hpp"
#include "creature-randomizer-context.hpp"

struct CreatureCreatingContext : public CreatureRandomizerContext {
    std::vector<int> m_vertices_connections;
    std::vector<bool> m_spring_is_muscle;
    std::set<int> m_variants;

    int m_vertex_count = 0;
    int m_spring_count = 0;
    int m_muscle_count = 0;
    int m_brain_layer_count = 0;
public:

    CreatureCreatingContext(const CreatureGenerator* generator, CreatureConfig* config):
            CreatureRandomizerContext(generator, config) {
    }

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