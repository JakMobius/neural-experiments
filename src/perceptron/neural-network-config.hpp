#pragma once

#include <vector>

struct NeuralNetworkConfig {
    int m_inner_neurons;

    std::vector<std::vector<double>> m_weights;
};