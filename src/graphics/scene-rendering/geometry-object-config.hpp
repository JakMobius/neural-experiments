#pragma once

#include <vector>
#include "../../utils/vec3.hpp"
#include "../../utils/packed-vec3.hpp"

struct SceneVertex {
    PackedVec3f m_position;
    PackedVec3f m_normal;
    PackedVec3f m_color;

    SceneVertex(const Vec3f& position, const Vec3f& normal, const Vec3f& color):
        m_position(position.x, position.y, position.z),
        m_normal(normal.x, normal.y, normal.z),
        m_color(color.x, color.y, color.z) {
    }

    // 9 floats + additional 32-bit matrix index
    static constexpr int length = 10;
    static constexpr int index_offset = 9;
};

struct SceneObjectConfig {
    std::vector<SceneVertex> m_mesh;
};