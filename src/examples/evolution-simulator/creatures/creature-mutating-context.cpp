//
// Created by Артем on 05.02.2022.
//

#include "creature-mutating-context.hpp"

void CreatureMutatingContext::mutate_vertex_position(VertexConfig &vertex) {
    auto new_position = vertex.m_position + random_vector({-0.1, -0.1, -0.1}, {0.1, 0.1, 0.1});

    for(int i = 0; i < 3; i++) {
        if(new_position[i] < m_generator->m_min_pos[i] || new_position[i] > m_generator->m_max_pos[i]){
            new_position.set(i, vertex.m_position[i]);
        }
    }

    vertex.m_position = new_position;
}

void CreatureMutatingContext::mutate_vertex_floor_friction(VertexConfig &vertex) {
    float new_floor_friction = vertex.m_floor_friction + random_float(-0.1, 0.1);
    if(vertex.m_floor_friction >= m_generator->m_min_friction && vertex.m_floor_friction <= m_generator->m_max_friction) {
        vertex.m_floor_friction = new_floor_friction;
    }
}

void CreatureMutatingContext::mutate_spring_strength(SpringConfig& spring) {
    float new_strength = spring.m_strength + random_float(-0.1, 0.1);
    if(new_strength >= m_generator->m_min_strength && new_strength <= m_generator->m_max_strength) {
        spring.m_strength = new_strength;
    }
}

void CreatureMutatingContext::mutate_brain(NeuralNetworkConfig& config) {
    auto mutations = random_int(m_generator->m_min_brain_murations, m_generator->m_max_brain_murations);

    for(int i = 0; i < mutations; i++) {
        auto layer_index = random_int(0, config.m_weights.size());
        auto weight_index = random_int(0, config.m_weights[layer_index].size());

        mutate_weight(config, layer_index, weight_index);
    }
}

void CreatureMutatingContext::mutate_weight(NeuralNetworkConfig &config, int layer_index, int weight_index) {

    int mutation_type = random_int(0, 4);
    double& weight = config.m_weights[layer_index][weight_index];

    switch(mutation_type) {
        case 0:
            weight += random_double(-0.5, 0.5);
            break;
        case 1:
            weight *= -1;
            break;
        case 2:
            weight *= random_double(0.5, 1.5);
            break;
        case 3:
            weight = random_double(-1, 1);
            break;
        default: break;
    }
}
