//
// Created by Артем on 05.02.2022.
//

#include "creature-mutator.hpp"


float CreatureMutator::random_float(float min, float max) const {
    return (float) m_rnd() / (float) std::mt19937::max() * (max - min) + min;
}

double CreatureMutator::random_double(double min, double max) const {
    return (double) m_rnd() / (double) std::mt19937::max() * (max - min) + min;
}

int CreatureMutator::random_int(int min, int max) const {
    if(min >= max) return max;
    return m_rnd() % (max - min) + min;
}

Vec3f CreatureMutator::random_vector(const Vec3f &min, const Vec3f &max) const {
    return Vec3f {
            random_float(min.x, max.x),
            random_float(min.y, max.y),
            random_float(min.z, max.z)
    };
}
