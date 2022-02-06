#pragma once

#include <vector>
#include "physics/physics-engine.hpp"
#include "../../graphics/scene-rendering/scene-renderer.hpp"
#include "objects/world-object.hpp"

class EvolutionWorld {
    std::unique_ptr<ConcurrentPhysicsEngine> m_physics_engine {};
    std::unique_ptr<Graphics::SceneRenderer> m_renderer {};
    std::unordered_set<WorldObject*> m_objects {};

    Graphics::GeometryObject* m_floor = nullptr;

public:
    void add_floor();

    EvolutionWorld() {
        m_physics_engine = std::make_unique<ConcurrentPhysicsEngine>();
        m_renderer = std::make_unique<Graphics::SceneRenderer>();

        add_floor();
    }

    void set_screen_size(const Vec2i& size) { m_renderer->set_screen_size(size); }
    Graphics::SceneRenderer* get_renderer() { return m_renderer.get(); }
    ConcurrentPhysicsEngine* get_physics_engine() { return m_physics_engine.get(); }

    void tick(float dt);
    void physics_tick(float dt);

    void draw() { m_renderer->draw(); }

    void add_object(WorldObject* object) { m_objects.insert(object); }
    void remove_object(WorldObject* object) { m_objects.erase(object); }
};