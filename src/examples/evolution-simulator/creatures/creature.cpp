//
// Created by Артем on 04.02.2022.
//

#include "creature.hpp"

Vec3f Creature::get_center() {
    Vec3f center = {0, 0, 0};

    for(auto vertex : m_vertices) {
        center += vertex->get_physics_vertex()->m_position;
    }

    center /= (float) m_vertices.size();

    return center;
}
