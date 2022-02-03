#pragma once

namespace Graphics {

class Scene;

}

#include <vector>
#include "scene-renderer.hpp"
#include "../camera.hpp"
#include "../../utils/vec2.hpp"

namespace Graphics {

class Scene {

    std::unique_ptr<SceneRenderer> m_renderer {};

    float phase = 0;

public:
    Scene();

    void set_screen_size(const Vec2i& screen_size);

    SceneRenderer* get_renderer() { return m_renderer.get(); }

    void set_camera(Camera* camera);

    void draw();
};

}