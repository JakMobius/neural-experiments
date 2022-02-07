#pragma once

#include "creature-config.hpp"
#include "creature-generator.hpp"
#include "creature-randomizer-context.hpp"

class CreatureMutatingContext : public CreatureRandomizerContext {

public:
    CreatureMutatingContext(const CreatureGenerator* generator, CreatureConfig* config):
        CreatureRandomizerContext(generator, config) {
    }

    void mutate_vertex_floor_friction(VertexConfig &vertex);
    void mutate_vertex_position(VertexConfig& vertex);
    void mutate_spring_strength(SpringConfig& config);
    void mutate_brain(NeuralNetworkConfig& config);

    void mutate_creature() {
        for(auto& vertex : m_result->m_vertices) {
            mutate_vertex_floor_friction(vertex);
            mutate_vertex_position(vertex);
        }

        for(auto& spring : m_result->m_springs) {
            mutate_spring_strength(spring);
        }

        mutate_brain(m_result->m_brain_config);
    }

    void mutate_weight(NeuralNetworkConfig &config, int layer_index, int weight_index);
};