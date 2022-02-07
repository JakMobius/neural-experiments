//
// Created by Артем on 06.02.2022.
//

#include "creature-randomizer-context.hpp"
#include "../../../utils/vec3.hpp"

float CreatureRandomizerContext::random_float(float min, float max) const {
    return (float) (m_generator->m_rnd)() / (float) std::mt19937::max() * (max - min) + min;
}

double CreatureRandomizerContext::random_double(double min, double max) const {
    return (double) (m_generator->m_rnd)() / (double) std::mt19937::max() * (max - min) + min;
}

int CreatureRandomizerContext::random_int(int min, int max) const {
    if(min >= max) return max;
    return m_generator->m_rnd() % (max - min) + min;
}

Vec3f CreatureRandomizerContext::random_vector(const Vec3f& min, const Vec3f& max) const {
    return Vec3f {
            random_float(min.x, max.x),
            random_float(min.y, max.y),
            random_float(min.z, max.z)
    };
}