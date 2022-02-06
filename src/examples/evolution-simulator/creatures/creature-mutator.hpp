#pragma once

#include "creature-config.hpp"
#include "creature-generator.hpp"

class CreatureMutator {

    mutable std::mt19937 m_rnd { 0 };

public:

    void mutate(CreatureConfig& config) {
        for(auto& vertex : config.m_vertices) {
//            vertex.m_mass += random_float(-0.05, 0.05);
            vertex.m_floor_friction += random_float(-0.1, 0.1);

            if(vertex.m_mass < 0.1) vertex.m_mass = 0.1;
            if(vertex.m_floor_friction < 0.1) vertex.m_floor_friction = 0.1;

            auto new_position = vertex.m_position + random_vector({-0.1, -0.1, -0.1}, {0.1, 0.1, 0.1});

            if(new_position.x < -0.5 || new_position.x > 0.5) new_position.x = vertex.m_position.x;
            if(new_position.y < 0.5 || new_position.y > 1) new_position.y = vertex.m_position.y;
            if(new_position.z < -0.5 || new_position.z > 0.5) new_position.z = vertex.m_position.z;

            vertex.m_position = new_position;
        }

        for(auto& spring : config.m_springs) {
            spring.m_strength += random_float(-0.1, 0.1);
        }

        for(auto& matrix : config.m_brain_config.m_weights){
            for(auto& weight : matrix) {
                weight += random_double(-0.01, 0.01);
            }
        }
    }

    float random_float(float min, float max) const;

    double random_double(double min, double max) const;

    int random_int(int min, int max) const;

    Vec3f random_vector(const Vec3f& min, const Vec3f& max) const;
};