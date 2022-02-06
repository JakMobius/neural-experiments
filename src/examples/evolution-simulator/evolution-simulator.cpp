//
// Created by Артем on 30.01.2022.
//

#include "evolution-simulator.hpp"
#include "creatures/creature-generator.hpp"

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

    float dt = 1.0f / 60.0f;
    int speed = 1;

    if(m_speedup) speed *= 20;

    for(int j = 0; j < speed; j++) {
        m_world->get_physics_engine()->tick();
        m_generation_time += dt;
    }

    m_world->tick(dt);

    if(m_generation_time > 10.0f) {
        m_generation_time = 0.0f;
        next_generation();
    }

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

    if(key == sf::Keyboard::F) m_speedup = !m_speedup;
}

void EvolutionSimulator::on_key_release(sf::Keyboard::Key key) {
    GeneralApp::on_key_release(key);
}

EvolutionSimulator::EvolutionSimulator() : GeneralApp() {
    create_window(2200, 1600);
    m_world = std::make_unique<EvolutionWorld>();
    m_camera = std::make_unique<PerspectiveCamera>();
    m_camera->set_position({0, 3, -3});

    m_world->get_renderer()->set_camera(m_camera.get());
    m_user_controller.set_controlled_camera(m_camera.get());
    auto size = m_window->getSize();
    m_world->set_screen_size({(int) size.x, (int) size.y});

    m_camera->set_aspect((float) size.x / (float) size.y);

    create_initial_generation();

    m_world->get_renderer()->add_light({{ 0.3, -0.8, 0.5 }, { 1, 1, 1 }});
}

void EvolutionSimulator::next_generation() {
    std::sort(m_creatures.begin(), m_creatures.end());
    std::cout << "Running generation " << m_generation_index++ << ", best creature score is " << m_creatures[0].m_creature->get_score() << "\n";

    int from = m_creatures.size() / 2;

    for(int i = 0; i < m_creatures.size(); i++) {
        if(i >= from) {
            m_creatures[i].m_config = m_creatures[i - from].m_config;
            m_creature_mutator.mutate(m_creatures[i].m_config);
        }

        delete m_creatures[i].m_creature;

        m_creatures[i].m_creature = new Creature(m_world.get(), m_creatures[i].m_config);
        if(i == 0) m_creatures[i].m_creature->make_visible();

        for(auto vertex : m_creatures[i].m_creature->get_vertices()) {
            vertex->get_physics_vertex()->m_position.x += (float)(i % 30) * 20;
            vertex->get_physics_vertex()->m_position.z += (float)(i / 30) * 20;
        }

        m_creatures[i].m_creature->fix_center();
    }
}

void EvolutionSimulator::create_initial_generation() {

    int count = 500;
    m_creatures.resize(count);

    for(int i = 0; i < count; i++) {

        m_creature_generator.generate_creature(i, &m_creatures[i].m_config);

        auto creature = new Creature(m_world.get(), m_creatures[i].m_config);

        for(auto vertex : creature->get_vertices()) {
            vertex->get_physics_vertex()->m_position.x += (float)(i % 30) * 20;
            vertex->get_physics_vertex()->m_position.z += (float)(i / 30) * 20;
        }

        m_creatures[i].m_creature = creature;
        m_creatures[i].m_creature->fix_center();
        if(i == 0) m_creatures[i].m_creature->make_visible();
    }
}
