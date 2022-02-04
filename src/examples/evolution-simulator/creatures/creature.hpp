#pragma once

#include <vector>
#include "../objects/vertex-object.hpp"
#include "../objects/spring-object.hpp"
#include "creature-config.hpp"

class Creature {
    std::vector<VertexObject*> m_vertices {};
    std::vector<SpringObject*> m_springs {};

public:
    Creature(EvolutionWorld* world, const CreatureConfig& config) {
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

            m_springs.push_back(spring);
        }
    }

    ~Creature() {
        for(auto& vertex : m_vertices) delete vertex;
        for(auto& spring : m_springs) delete spring;
    }

    Vec3f get_center();

    const std::vector<VertexObject*>& get_vertices() { return m_vertices; }
    const std::vector<SpringObject*>& get_springs() { return m_springs; }

};