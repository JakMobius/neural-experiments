//
// Created by Артем on 30.01.2022.
//

#include "evolution-simulator.hpp"
#include "../../graphics/scene-rendering/shape-generator.hpp"

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
    if(object) {
        phase += 0.01;

        auto matrix = Matrix4f::rotation_y_matrix(phase) * Matrix4f::translation_matrix(0, 0.3 * sin(phase * 2), 3);

        object->set_transform(matrix);
    }
    m_scene->draw();
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

    if(key == sf::Keyboard::LControl) remove_cube();
}

void EvolutionSimulator::on_key_release(sf::Keyboard::Key key) {
    GeneralApp::on_key_release(key);

    if(key == sf::Keyboard::LControl) add_cube();
}

void EvolutionSimulator::create_scene() {
    ShapeGenerator shape_generator {};

    shape_generator.add_cube({0, -2, 0}, {10, 0.1, 10}, {
            {0.7, 0.4, 0.5},
            {0.7, 0.4, 0.5},
            {0.7, 0.4, 0.5},
            {0.7, 0.4, 0.5},
            {0.7, 0.4, 0.5},
            {0.7, 0.4, 0.5},
    });

    m_scene->get_renderer()->get_geometry_pool()->create_object({shape_generator.get_mesh()}, nullptr);
    shape_generator.reset();

    m_scene->get_renderer()->add_light({{-0.25, -1, -0.5}, {1, 1, 1}});
}

void EvolutionSimulator::add_cube() {
    ShapeGenerator generator;

    generator.add_cube({0, 0, 0}, {1, 1, 1}, {1, 1, 1});
    object = m_scene->get_renderer()->get_geometry_pool()->create_object({generator.get_mesh()}, nullptr);
    generator.reset();

    for(int i = 0; i < 3; i++) {
        generator.add_cube({(float)(i-1) / 2, -1, 0}, {0.1, 0.1, 0.1}, {1, 1, 1});
        m_scene->get_renderer()->get_geometry_pool()->create_object({generator.get_mesh()}, object);
        generator.reset();
    }
}

void EvolutionSimulator::remove_cube() {
    if(!object) return;
    m_scene->get_renderer()->get_geometry_pool()->remove_object(object);
    object = nullptr;
}
