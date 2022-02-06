#pragma once

#include "spring-object.hpp"

struct Muscle {
    SpringObject* m_spring = nullptr;
    float m_min_length = 0.5;
    float m_max_length = 2.0;

    Muscle() {}

    void update_input(float input) const;
};