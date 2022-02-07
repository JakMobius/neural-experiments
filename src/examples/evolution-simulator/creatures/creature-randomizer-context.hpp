#pragma once

#include <climits>
#include "../../../utils/vec3.hpp"
#include "creature-generator.hpp"
#include "creature-config.hpp"

class CreatureRandomizerContext {
protected:
    float random_float(float min = 0, float max = 1) const;
    double random_double(double min = 0, double max = 1) const;
    int random_int(int min = INT_MIN, int max = INT_MAX) const;
    Vec3f random_vector(const Vec3f &min, const Vec3f &max) const;

    CreatureConfig* m_result;
    const CreatureGenerator* m_generator;

public:
    CreatureRandomizerContext(const CreatureGenerator* generator, CreatureConfig* config):
            m_generator(generator), m_result(config) {
    }
};