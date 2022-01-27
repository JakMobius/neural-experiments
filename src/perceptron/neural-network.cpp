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

//        std::random_device dev;
//        std::mt19937 rng(dev());
    std::mt19937 rng(0);

    for(int i = 1; i < m_layers.size(); i++) {
        m_layers[i - 1]->set_next_layer(m_layers[i]);
        m_layers[i - 1]->randomize(rng);
    }
}
