//
// Created by Артем on 30.01.2022.
//

#include "bitmap-app.hpp"

void BitmapApp::create_window(unsigned int width, unsigned int height, int scale) {
    m_window_width = width;
    m_window_height = height;
    m_scale = scale;

    m_window = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(m_window_width * m_scale, m_window_height * m_scale, 32), "Neural Network");
    m_window->setFramerateLimit(60);

    m_pixels = std::make_unique<sf::Uint8[]>(m_window_width * m_window_height * 4);

    m_sprite.setScale((float) m_scale, (float) m_scale);

    m_texture.create(m_window_width, m_window_height);
    m_sprite.setTexture(m_texture);
}

Vec2f BitmapApp::window_coordinates(const Vec2d &space_coordinates) const {
    return (Vec2f) (space_coordinates * ((double)m_window_width * (double)m_scale));
}

Vec2d BitmapApp::space_coordinates(const Vec2f &window_coordinates) const {
    return ((Vec2d) window_coordinates) / ((double)m_window_width * (double)m_scale);
}

void BitmapApp::on_tick() {
    m_window->clear();
    m_texture.update(m_pixels.get());
    m_window->draw(m_sprite);

    on_draw();

    m_window->display();
}