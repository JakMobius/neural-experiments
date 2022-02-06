#pragma once

#include <vector>
#include "../objects/vertex-object.hpp"
#include "../objects/spring-object.hpp"
#include "creature-config.hpp"
#include "../objects/muscle.hpp"
#include "../../../perceptron/neural-network.hpp"
#include "../../../activators/sigmoid.hpp"
#include "../../../utils/vec2.hpp"

class Creature : public WorldObject {
    std::vector<VertexObject*> m_vertices {};
    std::vector<SpringObject*> m_springs {};
    std::vector<Muscle*> m_muscles {};

    NeuralNetwork* m_brain = nullptr;

    float m_time = 0;

    Vec3f m_start_position {};

public:
    Creature(EvolutionWorld* world, const CreatureConfig& config);

    ~Creature() override;

    Vec3f get_center() const;

    const std::vector<VertexObject*>& get_vertices() { return m_vertices; }
    const std::vector<SpringObject*>& get_springs() { return m_springs; }

    void tick(float dt) override;
    void physics_tick();

    void fix_center() {
        m_start_position = get_center();
    }

    float get_score() const {
        auto center = get_center();

        if(center.y <= 0.25) return 0;

        return Vec2f(center.x - m_start_position.x, center.z - m_start_position.z).len();

//        return result;
    }

    void make_visible() {
        for(auto& vertex : m_vertices) {
            float friction = vertex->get_physics_vertex()->m_floor_friction;
            friction = (friction - 0.01) / (2.0 - 0.01);

            vertex->create_colored_mesh({1, 1 - friction, 1 - friction});
        }

        for(auto& muscle : m_muscles) {
            float strength = muscle->m_spring->get_physics_spring()->m_strength;
            strength = (strength - 25.0) / (100.0 - 25.0);

            muscle->m_spring->create_colored_mesh({1 - strength, 1, 1 - strength});
        }
        for(auto& spring : m_springs) {
            float strength = spring->get_physics_spring()->m_strength;
            strength = (strength - 25.0) / (100.0 - 25.0);

            if(!spring->get_geometry_object()) spring->create_colored_mesh({1 - strength, 1 - strength, 1});
        }
    }
};