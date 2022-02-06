//
// Created by Артем on 24.01.2022.
//

#include "neural-network.hpp"

NeuralNetwork::NeuralNetwork(int inner_neurons, int outer_neurons, const std::vector<int> &hidden_layers): m_errors(outer_neurons, 0) {
    m_layers.push_back(new Layer(inner_neurons));

    for(int hidden_layer : hidden_layers) {
        m_layers.push_back(new Layer(hidden_layer));
    }

    m_layers.push_back(new Layer(outer_neurons));

    for(int i = 1; i < m_layers.size(); i++) {
        m_layers[i - 1]->set_next_layer(m_layers[i]);
    }
}

NeuralNetwork::NeuralNetwork(const NeuralNetworkConfig &config) {
    m_layers.push_back(new Layer(config.m_inner_neurons));

    int neurons = config.m_inner_neurons;

    for(auto& weights : config.m_weights) {
        neurons = (int) weights.size() / (neurons + 1);
        auto layer = new Layer(neurons);
        m_layers.back()->set_next_layer(layer);
        m_layers.back()->set_transition_matrix(weights);
        m_layers.push_back(layer);
    }
}
