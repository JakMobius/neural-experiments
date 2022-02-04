//
// Created by Артем on 30.01.2022.
//

#include "evolution-simulator.hpp"

void EvolutionSimulator::on_tick() {
    reset_gl();
    clear_window();
    on_draw();

    m_user_controller.on_tick();
    m_window->display();
}


void EvolutionSimulator::on_mouse_move(double x, double y) {
    m_user_controller.on_mouse_move(x, y);
}

void EvolutionSimulator::on_draw() {
    m_world->tick(1.0f / 60.0f);
    m_world->draw();
}

void EvolutionSimulator::create_window(int width, int height) {

    sf::ContextSettings settings;
    settings.majorVersion = 4;
    settings.minorVersion = 1;
    settings.depthBits = 24;
    settings.attributeFlags |= sf::ContextSettings::Core;

    m_window = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(width, height, 32),
            "3D Stuff",
            sf::Style::Titlebar | sf::Style::Close,
            settings
    );

    m_window->setFramerateLimit(60);
    m_window->setVerticalSyncEnabled(true);

    glewInit();

    GLException::enable_debug();
    GLException::check();
}

void EvolutionSimulator::reset_gl() {
    auto size = m_window->getSize();
    glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);
}

void EvolutionSimulator::clear_window() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EvolutionSimulator::on_key_press(sf::Keyboard::Key key) {
    GeneralApp::on_key_press(key);


}

void EvolutionSimulator::on_key_release(sf::Keyboard::Key key) {
    GeneralApp::on_key_release(key);
}

EvolutionSimulator::EvolutionSimulator() : GeneralApp() {
    create_window(1600, 1600);
    m_world = std::make_unique<EvolutionWorld>();
    m_camera = std::make_unique<PerspectiveCamera>();

    m_camera->set_position({0, 3, -3});

    m_world->get_renderer()->set_camera(m_camera.get());
    m_user_controller.set_controlled_camera(m_camera.get());
    auto size = m_window->getSize();
    m_world->set_screen_size({(int) size.x, (int) size.y});

    for(int i = 0; i < 500; i++) {
        add_creature();
    }

    m_world->get_renderer()->add_light({{ 0.3, -0.8, 0.5 }, { 1, 1, 1 }});
}

void EvolutionSimulator::add_creature() {

    float randomized = (float)rng() / (float)std::mt19937::max();

    Vec3f offset = { 170 + randomized, 0, 10 + randomized };

    std::vector<VertexConfig> vertices_config {
            { 0.5f, 10.0f, offset + Vec3f {2, 0, -2} },
            { 0.5f, 10.0f, offset + Vec3f {2, 0, 2} },
            { 0.5f, 10.0f, offset + Vec3f {-2, 0, -2} },
            { 0.5f, 10.0f, offset + Vec3f {0, 2, 0} }
    };

    std::vector<SpringConfig> springs_config {
            { randomized + 50.0f, randomized + 5.0f,3.0f, 0, 1 },
            { randomized + 50.0f, randomized + 5.0f,3.0f, 1, 2 },
            { randomized + 50.0f, randomized + 5.0f,3.0f, 2, 0 },
            { randomized + 50.0f, randomized + 5.0f,3.0f, 0, 3 },
            { randomized + 50.0f, randomized + 5.0f,3.0f, 1, 3 },
            { randomized + 50.0f, randomized + 5.0f,3.0f, 2, 3 }
    };

    CreatureConfig config {
            std::move(vertices_config),
            std::move(springs_config)
    };

    auto creature = new Creature(m_world.get(), config);

    for(auto vertex : creature->get_vertices()) {
        vertex->get_physics_vertex()->m_velocity = {-90 + randomized, 3 + randomized, 0};
    }

    if(!m_creature) m_creature = creature;
}
