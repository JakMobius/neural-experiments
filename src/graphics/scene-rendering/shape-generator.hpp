#pragma once

#include "../../utils/vec3.hpp"
#include "geometry-object-config.hpp"
#include "material.hpp"

namespace Graphics {
class ShapeGenerator {

    std::vector<Graphics::SceneVertex> m_mesh {};

public:

    std::vector<Graphics::SceneVertex> &get_mesh() { return m_mesh; }

    void reset();;

    void add_triangle(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, Graphics::Material* material);

    void add_square(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &p4, Material* material);

    void add_cube(const Vec3f &center, const Vec3f &size, Material* material);

    void add_cube(const Vec3f &center, const Vec3f &size, Material* materials[6]);

    void add_vertex(const Vec3f &position, const Vec3f &normal, Material* material);
};
}