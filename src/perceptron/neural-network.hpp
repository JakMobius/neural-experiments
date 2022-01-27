#pragma once

class NeuralNetwork;

#include <iostream>
#include "layer.hpp"

class NeuralNetwork {

    std::vector<Layer*> m_layers = {};
    std::vector<double> m_errors;
    double m_learning_rate = 0.000005;

public:
    NeuralNetwork(int inner_neurons, int outer_neurons, const std::vector<int>& hidden_layers);

    double get_learning_rate() const {
        return m_learning_rate;
    }

    void set_learning_rate(double learning_rate) {
        m_learning_rate = learning_rate;
    }

    template<typename Activator>
    void evaluate() {
        for(int i = 0; i < m_layers.size() - 1; i++) {
            m_layers[i]->propagate<Activator>();
        }
    }

    template<typename Activator>
    void descent() {
        auto layer_count = m_layers.size();

        std::vector<double> errors(m_errors);
        std::vector<double> new_errors;
        std::vector<double> gradients;

        for(int layer_index = layer_count - 2; layer_index >= 0; layer_index--) {
            auto layer = m_layers[layer_index];
            auto neurons = layer->get_neurons();
            auto values = layer->get_values();

            auto next_layer = m_layers[layer_index + 1];
            auto next_neurons = next_layer->get_neurons();
            auto next_values = next_layer->get_values();

            if(gradients.size() < next_neurons) gradients.resize(next_neurons);
            if(errors.size() < next_neurons) errors.resize(next_neurons);
            if(new_errors.size() < neurons) new_errors.resize(neurons);

            for(int j = 0; j < next_neurons; j++) {
                gradients[j] = errors[j] * Activator::derivative(next_values[j]) * m_learning_rate;
            }

            for (int i = 0; i < neurons; i++) {
                double value = 0;
                for (int j = 0; j < next_neurons; j++) {
                    value += layer->get_transition(i, j) * errors[j];
                }
                new_errors[i] = value;
            }

            std::swap(errors, new_errors);

            for (int j = 0; j < next_neurons; j++) {
                double gradient = gradients[j];

                for (int i = 0; i < neurons; i++) {
                    layer->get_transition(i, j) += values[i] * gradient;
                }

                layer->get_bias(j) += gradient;
            }
        }
    }

    std::vector<double>& get_input() {
        return m_layers[0]->get_values();
    }

    std::vector<double>& get_errors() {
        return m_errors;
    }

    const std::vector<double>& get_output() {
        return m_layers[m_layers.size() - 1]->get_values();
    }
};