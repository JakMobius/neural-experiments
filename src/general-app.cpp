//
// Created by Артем on 27.01.2022.
//

#include "general-app.hpp"

void GeneralApp::process_event(const sf::Event &event) {
    switch(event.type) {
        case sf::Event::Closed: m_window->close(); break;
        case sf::Event::KeyPressed:
            on_key_press(event.key.code);
            break;
        case sf::Event::KeyReleased:
            on_key_release(event.key.code);
            break;
        case sf::Event::MouseButtonPressed:
            on_mouse_press((double)event.mouseButton.x / (double)m_window_width / (double)m_scale, (double)event.mouseButton.y / (double)m_window_height / (double)m_scale);
            break;
        default: break;
    }
}

void GeneralApp::create_window(unsigned int width, unsigned int height, int scale) {
    m_window_width = width;
    m_window_height = height;
    m_scale = scale;

    m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(m_window_width * m_scale, m_window_height * m_scale, 32), "Neural Network");
    m_pixels = std::make_unique<sf::Uint8[]>(m_window_width * m_window_height * 4);
    m_window->setFramerateLimit(60);
}

void GeneralApp::loop() {

    sf::Texture texture;
    sf::Sprite sprite;
    sf::Event event = {};

    sprite.setScale((float)m_scale, (float)m_scale);

    texture.create(m_window_width, m_window_height);
    sprite.setTexture(texture);

    while(m_window->isOpen()) {

        while (m_window->pollEvent(event)) {
            process_event(event);
        }

        on_tick();

        m_window->clear();
        texture.update(m_pixels.get());
        m_window->draw(sprite);

        on_draw();

        m_window->display();
    }
}

sf::Vector2<float> GeneralApp::window_coordinates(const sf::Vector2<double> &space_coordinates) {
    return (sf::Vector2<float>) (space_coordinates * ((double)m_window_width * (double)m_scale));
}

sf::Vector2<double> GeneralApp::space_coordinates(const sf::Vector2<float> &window_coordinates) {
    return ((sf::Vector2<double>) window_coordinates) / ((double)m_window_width * (double)m_scale);
}
