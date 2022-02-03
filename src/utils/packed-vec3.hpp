#pragma once

template<typename T>
struct PackedVec3 {
    T x, y, z;

    PackedVec3(T x, T y, T z): x(x), y(y), z(z) {}
};

typedef PackedVec3<double> PackedVec3d;
typedef PackedVec3<float> PackedVec3f;