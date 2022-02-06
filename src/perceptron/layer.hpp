#pragma once

#include <vector>
#include <random>

class Layer {
    Layer* m_next_layer = nullptr;
    Layer* m_previous_layer = nullptr;

    std::vector<double> m_transition_matrix = {};
    std::vector<double> m_values = {};
    int m_neurons = 0;

    void calculate_raw_layer_values();

public:
    Layer(int neurons): m_neurons(neurons), m_values(neurons) {

    }

    template<typename Activator>
    void propagate() {

        calculate_raw_layer_values();

        for(auto &value : m_next_layer->get_values()) {
            value = Activator::value(value);
        }
    }

    void randomize(std::mt19937& rng);

    void set_next_layer(Layer* layer);

    int get_neurons() const { return m_neurons; }

    inline double& get_transition(int neuron_index, int target_neuron_index) {
        return m_transition_matrix[neuron_index + target_neuron_index * (m_neurons + 1)];
    }

    inline double& get_bias(int target_neuron_index) {
        return m_transition_matrix[m_neurons + target_neuron_index * (m_neurons + 1)];
    }

    std::vector<double>& get_values() { return m_values; }
    const std::vector<double>& get_values() const { return m_values; }

    void set_transition_matrix(const std::vector<double>& matrix) { m_transition_matrix = matrix; }
    std::vector<double>& get_transition_matrix() { return m_transition_matrix; }
    const std::vector<double>& get_transition_matrix() const { return m_transition_matrix; }
};