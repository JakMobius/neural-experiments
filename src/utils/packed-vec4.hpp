#pragma once

template<typename T>
struct PackedVec4 {
    T x, y, z, w;

    PackedVec4(T x, T y, T z, T w): x(x), y(y), z(z), w(w) {}
};

typedef PackedVec4<double> PackedVec4d;
typedef PackedVec4<float> PackedVec4f;