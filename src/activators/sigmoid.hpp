#pragma once

#include <cmath>

struct Sigmoid {
    static double value(double x) {
        return (double)1 / (1 + exp(-x));
    }

    static double derivative(double y) {
        return y * (1 - y);
    }
};