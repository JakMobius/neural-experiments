//
// Created by Артем on 04.02.2022.
//

#include "creature-creating-context.hpp"

float CreatureCreatingContext::random_float(float min, float max) const {
    return (float) (m_generator->m_rnd)() / (float) std::mt19937::max() * (max - min) + min;
}

double CreatureCreatingContext::random_double(double min, double max) const {
    return (double) (m_generator->m_rnd)() / (double) std::mt19937::max() * (max - min) + min;
}

int CreatureCreatingContext::random_int(int min, int max) const {
    if(min >= max) return max;
    return m_generator->m_rnd() % (max - min) + min;
}

Vec3f CreatureCreatingContext::random_vector(const Vec3f& min, const Vec3f& max) const {
    return Vec3f {
            random_float(min.x, max.x),
            random_float(min.y, max.y),
            random_float(min.z, max.z)
    };
}

VertexConfig CreatureCreatingContext::random_vertex() {
    VertexConfig config {};
    config.m_mass = random_float(m_generator->m_min_mass, m_generator->m_max_mass);
    config.m_floor_friction = random_float(m_generator->m_min_friction, m_generator->m_max_friction);
    config.m_position = random_vector(m_generator->m_min_pos, m_generator->m_max_pos);

    return config;
}

SpringConfig CreatureCreatingContext::random_spring() {
    SpringConfig config {};

    config.m_damping = random_float(m_generator->m_min_damping, m_generator->m_max_damping);
    config.m_strength = random_float(m_generator->m_min_strength, m_generator->m_max_strength);

    return config;
}

MuscleConfig CreatureCreatingContext::random_muscle() {
    MuscleConfig config {};

    // There is no point in randomizing this
    config.m_max_length_coefficient = 2.0;
    config.m_min_length_coefficient = 0.5;
    config.m_spring_index = random_muscle_index();

    m_spring_is_muscle[config.m_spring_index] = true;

    return config;
}

int CreatureCreatingContext::random_muscle_index() {
    int possible_muscles = m_spring_count - m_result->m_muscles.size();

    int muscle_index = random_int(0, possible_muscles);

    for(int i = 0; i < m_spring_count; i++) {
        if(!m_spring_is_muscle[i]) {
            if(muscle_index-- == 0) {
                return i;
            }
        }
    }

    return -1;
}

void CreatureCreatingContext::tension_spring(SpringConfig &spring, int v1, int v2) {
    spring.m_vertex_a_index = v1;
    spring.m_vertex_b_index = v2;

    m_vertices_connections[v1]++;
    m_vertices_connections[v2]++;
}

bool CreatureCreatingContext::tension_spring_randomly(SpringConfig &spring) {

    for(int i = 0; i < m_result->m_vertices.size(); i++) {
        if(m_vertices_connections[i] < m_result->m_vertices.size() - 1) m_variants.insert(i);
    }

    if(m_variants.size() == 1) {
        std::cout << "That's weird" << std::endl;
    }

    if(m_variants.size() == 0) {
        m_variants.clear();
        return false;
    }

    /* TODO: There is a room for improvement here. std::advance works in O(n) time, however
       theoretically it's possible to choose random element from std::set in O(logn) time. */

    auto v1_it = m_variants.begin();
    std::advance(v1_it, random_int(0, (int)m_variants.size()));
    int v1 = *v1_it;

    m_variants.clear();
    return tension_spring_randomly(spring, v1);
}

bool CreatureCreatingContext::tension_spring_randomly(SpringConfig &spring, int v1) {
    for(int i = 0; i < m_result->m_vertices.size(); i++) {
        if(m_vertices_connections[i] < m_result->m_vertices.size() - 1 && i != v1) {
            m_variants.insert(i);
        }
    }

    for(auto each_spring : m_result->m_springs) {
        if(each_spring.m_vertex_a_index == v1) m_variants.erase(each_spring.m_vertex_b_index);
        if(each_spring.m_vertex_b_index == v1) m_variants.erase(each_spring.m_vertex_a_index);
    }

    if(m_variants.empty()) {
        m_variants.clear();
        return false;
    }

    auto v2_it = m_variants.begin();
    std::advance(v2_it, random_int(0, (int)m_variants.size()));
    int v2 = *v2_it;

    tension_spring(spring, v1, v2);

    m_variants.clear();
    return true;
}

void CreatureCreatingContext::generate_creature() {

    m_result->m_springs.clear();
    m_result->m_vertices.clear();

    m_vertex_count = random_int(m_generator->m_min_vertices, m_generator->m_max_vertices);

    int min_springs = (m_vertex_count * 3 + 1) / 2;
    int max_springs = (m_vertex_count * (m_vertex_count - 1) + 1) / 2;
    m_spring_count = random_int(min_springs, max_springs);

    m_vertices_connections.resize(m_vertex_count);
    m_spring_is_muscle.resize(m_spring_count);

    generate_vertices();
    generate_springs();
    generate_muscles();
    generate_brain();
}

void CreatureCreatingContext::generate_springs() {


    for(int i = 0; i < m_vertex_count; i++) {
        while(m_vertices_connections[i] < 3) {
            SpringConfig spring = random_spring();

            tension_spring_randomly(spring, i);
            m_result->m_springs.push_back(spring);
        }
    }

    while(m_result->m_springs.size() < m_spring_count) {
        SpringConfig spring = random_spring();

        if(!tension_spring_randomly(spring)) {
            m_spring_count = m_result->m_springs.size();
            break;
        }
        m_result->m_springs.push_back(spring);
    }
}

void CreatureCreatingContext::generate_vertices() {

    for(int i = 0; i < m_vertex_count; i++) {
        m_result->m_vertices.push_back(random_vertex());

        if(i > 0) {
            int vertex_index = random_int(0, i);

            SpringConfig spring = random_spring();
            tension_spring(spring, vertex_index, i);
            m_result->m_springs.push_back(spring);
        }
    }
}

void CreatureCreatingContext::generate_muscles() {
    m_muscle_count = random_int(m_generator->m_min_muscles, m_generator->m_max_muscles);

    if(m_muscle_count > m_spring_count) m_muscle_count = m_spring_count;

    for(int i = 0; i < m_muscle_count; i++) {
        MuscleConfig muscle = random_muscle();

        m_result->m_muscles.push_back(muscle);
    }
}

void CreatureCreatingContext::generate_brain() {
    m_brain_layer_count = random_int(m_generator->m_min_layers, m_generator->m_max_layers);

    int input_neurons_count = m_muscle_count;
    int output_neurons_count = m_muscle_count;

    int neurons_count = input_neurons_count;

    std::vector<std::vector<double>> weights;

    for(int i = 0; i <= m_brain_layer_count; i++) {

        int next_neurons_count = 0;

        if(i < m_brain_layer_count) next_neurons_count = input_neurons_count;
        else next_neurons_count = output_neurons_count;

        weights.emplace_back((neurons_count + 1) * next_neurons_count, 0);
        for(auto& weight : weights.back()) weight = random_double(0, 1);

        neurons_count = next_neurons_count;
    }

    m_result->m_brain_config = { input_neurons_count, std::move(weights) };
}
