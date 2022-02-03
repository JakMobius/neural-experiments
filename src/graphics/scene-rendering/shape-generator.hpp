#pragma once

#include "../../utils/vec3.hpp"
#include "scene-object-config.hpp"

class ShapeGenerator {

    std::vector<SceneVertex> m_mesh {};

public:

    std::vector<SceneVertex>& get_mesh() { return m_mesh; }

    void reset() {
        m_mesh.clear();
    };

    void add_triangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &color) {
        Vec3f a = p3 - p1;
        Vec3f b = p2 - p1;
        Vec3f normal = a.cross(b);
        normal.normalize();
        add_vertex(p1, normal, color);
        add_vertex(p2, normal, color);
        add_vertex(p3, normal, color);
    }

    void add_square(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &p4, const Vec3f &color) {
        add_triangle(p1, p2, p4, color);
        add_triangle(p1, p4, p3, color);
    }

    void add_cube(const Vec3f &center, const Vec3f &size, const Vec3f color) {
        add_cube(center, size, {color, color, color, color, color, color});
    }

    void add_cube(const Vec3f &center, const Vec3f &size, const Vec3f (&colors)[6]) {

        Vec3f masks[3] = {
                {1, 0, 0},
                {0, 1, 0},
                {0, 0, 1}
        };
        for(int i = 0; i < 3; i++) {

            Vec3f basis_u = masks[(i + 1) % 3] * size;
            Vec3f basis_v = masks[(i + 2) % 3] * size;

            Vec3f offset = center + masks[i] * size * 0.5 - (basis_u + basis_v) * 0.5;

            add_square(offset,
                       offset + basis_u,
                       offset + basis_v,
                       offset + basis_u + basis_v, colors[i]);

            offset -= masks[i] * size;

            add_square(offset,
                       offset + basis_u,
                       offset + basis_v,
                       offset + basis_u + basis_v, colors[i + 3]);
        }
    }

    void add_vertex(const Vec3f& position, const Vec3f& normal, const Vec3f& color) {
        m_mesh.emplace_back(position, normal, color);
    }
};