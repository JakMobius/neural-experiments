#pragma once

template<typename T>
struct PackedVec2 {
    T x, y;

    PackedVec2(T x, T y): x(x), y(y) {}
};

typedef PackedVec2<double> PackedVec2d;
typedef PackedVec2<float> PackedVec2f;