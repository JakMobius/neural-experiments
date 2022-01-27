//
// Created by Артем on 24.01.2022.
//

#include "layer.hpp"

void Layer::randomize(std::mt19937 &rng) {
    int next_layer_neurons = m_next_layer->m_neurons;
    for(int i = 0; i < m_neurons + 1; i++) {
        for(int j = next_layer_neurons - 1; j >= 0; j--) {
            get_transition(i, j) = (double)(rng()) / (double)std::mt19937::max() * 2 - 1;
        }
    }
}

void Layer::calculate_raw_layer_values() {
    std::vector<double>& next_layer_values = m_next_layer->get_values();
    int next_neurons = next_layer_values.size();

    for(int j = 0; j < next_neurons; j++) {
        next_layer_values[j] = get_bias(j);
    }

    for(int i = 0; i < m_neurons; i++) {
        double value = m_values[i];
        for(int j = 0; j < next_neurons; j++) {
            next_layer_values[j] += value * get_transition(i, j);
        }
    }
}

void Layer::set_next_layer(Layer* layer) {
    if(m_next_layer) {
        m_next_layer->m_previous_layer = nullptr;
    }
    m_next_layer = layer;
    if(m_next_layer != nullptr) {
        m_next_layer->m_previous_layer = this;
        m_transition_matrix.resize(m_next_layer->get_neurons() * (m_neurons + 1));
    } else {
        m_transition_matrix.clear();
    }
}
