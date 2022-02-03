#pragma once

#include "../../general-app.hpp"
#include "../../graphics/scene-rendering/programs/scene-rendering-program.hpp"
#include "../../utils/vec2.hpp"
#include "../../graphics/scene-rendering/scene.hpp"
#include "user-controller.hpp"
#include "../../graphics/perspective_camera.hpp"

class EvolutionSimulator : public GeneralApp {

    UserController m_user_controller { (GeneralApp*) this };

    std::unique_ptr<Graphics::Scene> m_scene {};
    std::unique_ptr<PerspectiveCamera> m_camera {};

    Graphics::GeometryObject* object = nullptr;

    float phase = 0;

    void create_window(int width, int height);

    void reset_gl();
    void clear_window();

public:
    void create_scene();

    EvolutionSimulator(): GeneralApp() {
        create_window(1600, 1600);
        m_scene = std::make_unique<Graphics::Scene>();
        m_camera = std::make_unique<PerspectiveCamera>();

        m_scene->set_camera(m_camera.get());
        m_user_controller.set_controlled_camera(m_camera.get());
        auto size = m_window->getSize();
        m_scene->set_screen_size({ (int) size.x, (int) size.y });
        create_scene();
        add_cube();
    }

    void add_cube();
    void remove_cube();

    void on_tick() override;

    void on_mouse_move(double x, double y) override;

    void on_draw() override;

    void on_key_press(sf::Keyboard::Key key) override;
    void on_key_release(sf::Keyboard::Key key) override;
};