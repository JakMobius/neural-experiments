//
// Created by Артем on 04.02.2022.
//

#include "creature.hpp"
#include "../world.hpp"

Vec3f Creature::get_center() const {
    Vec3f center = {0, 0, 0};

    for(auto vertex : m_vertices) {
        center += vertex->get_physics_vertex()->m_position;
    }

    center /= (float) m_vertices.size();

    return center;
}

Creature::Creature(EvolutionWorld* world, const CreatureConfig &config) : WorldObject(world) {
    for(auto& vertex_config : config.m_vertices) {
        auto vertex = new VertexObject(world);
        vertex_config.apply(vertex);
        m_vertices.push_back(vertex);
    }

    for(auto& spring_config : config.m_springs) {
        auto spring = new SpringObject(world);
        auto physics_spring = spring->get_physics_spring();
        spring_config.apply(spring);

        physics_spring->m_vertex_a = m_vertices[spring_config.m_vertex_a_index]->get_physics_vertex();
        physics_spring->m_vertex_b = m_vertices[spring_config.m_vertex_b_index]->get_physics_vertex();
        physics_spring->m_target_length = physics_spring->get_length();

        m_springs.push_back(spring);
    }

    for(auto& muscle_config : config.m_muscles) {
        auto muscle = new Muscle();
        muscle_config.apply(muscle, m_springs[muscle_config.m_spring_index]);
        m_muscles.push_back(muscle);
    }

    m_brain = new NeuralNetwork(config.m_brain_config);

    m_world->get_physics_engine()->add_creature(this);
    m_world->add_object(this);
}

Creature::~Creature() {
    m_world->get_physics_engine()->remove_creature(this);
    for(auto& vertex : m_vertices) delete vertex;
    for(auto& spring : m_springs) delete spring;
    for(auto& muscle : m_muscles) delete muscle;
    delete m_brain;
    m_world->remove_object(this);
}

void Creature::tick(float dt) {
    m_time += dt;
}

void Creature::physics_tick() {
    auto& input = m_brain->get_input();

    for(int i = 0; i < m_muscles.size(); i++) {
        auto& muscle = m_muscles[i];
        auto physics_spring = muscle->m_spring->get_physics_spring();
        input[i] = (physics_spring->get_length() - muscle->m_min_length) / (muscle->m_max_length - muscle->m_min_length);
    }

    m_brain->evaluate<Sigmoid>();

    auto& output = m_brain->get_output();

    for(int i = 0; i < m_muscles.size(); i++) {
        m_muscles[i]->update_input((float) output[i]);
    }
}
