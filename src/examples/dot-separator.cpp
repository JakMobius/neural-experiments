//
// Created by Артем on 25.01.2022.
//

#include "dot-separator.hpp"
#include "../utils.hpp"

DotSeparator::DotSeparator() : BitmapApp() {
    create_window(200, 200, 4);

    m_net = std::make_unique<NeuralNetwork>(2, 1, std::vector<int> {
        16, 16, 16, 16
    });

    m_net->randomize(0);
    m_net->set_learning_rate(0.01);

    for(int i = 0; i < 10; i++) {
        m_white_points.emplace_back(-0.5 + 0.1 * i, 0.1 + sin((double)i * 0.7) * 0.15);
        m_black_points.emplace_back(-0.5 + 0.1 * i, sin((double)i * 0.7) * 0.15);
        m_white_points.emplace_back(-0.5 + 0.1 * i, -0.1 + sin((double)i * 0.7) * 0.15);
    }

    m_white_point.setFillColor({255, 255, 255, 255});
    m_black_point.setFillColor({0, 0, 0, 255});
    m_white_point.setOutlineColor({0, 255, 0, 255});
    m_black_point.setOutlineColor({0, 255, 0, 255});
    m_white_point.setOutlineThickness(2);
    m_black_point.setOutlineThickness(2);

    m_white_point.setOrigin(10, 10);
    m_black_point.setOrigin(10, 10);
}

void DotSeparator::learn() {
    auto& inputs = m_net->get_input();
    auto& outputs = m_net->get_output();
    auto& errors = m_net->get_errors();

    for(int i = 0; i < 2000; i++) {

        for(auto& point : m_black_points) {
            inputs[0] = point.x;
            inputs[1] = point.y;

            m_net->evaluate<Sigmoid>();
            errors[0] = 0.0 - outputs[0];
            m_net->descent<Sigmoid>();
        }

        for(auto& point : m_white_points) {
            inputs[0] = point.x;
            inputs[1] = point.y;

            m_net->evaluate<Sigmoid>();
            errors[0] = 1.0 - outputs[0];
            m_net->descent<Sigmoid>();
        }
    }
}

void DotSeparator::redraw() {
    auto& inputs = m_net->get_input();
    auto& outputs = m_net->get_output();

    for(int i = 0; i < m_window_height * m_window_width; i++) {
        auto rel_x = (double)(i % m_window_width) / (double)m_window_width;
        auto rel_y = (double)(i / m_window_width) / (double)m_window_height;

        inputs[0] = (rel_x - 0.5) * m_camera_scale;
        inputs[1] = (rel_y - 0.5) * m_camera_scale;

        m_net->evaluate<Sigmoid>();

        m_pixels[i * 4 + 0] = to_brightness(outputs[0]);
        m_pixels[i * 4 + 1] = to_brightness(outputs[0]);
        m_pixels[i * 4 + 2] = to_brightness(outputs[0]);
        m_pixels[i * 4 + 3] = 255;
    }
}

void DotSeparator::on_key_press(sf::Keyboard::Key key) {
    switch(key) {
        case sf::Keyboard::Up: adjust_learning_rate(1.25); break;
        case sf::Keyboard::Down: adjust_learning_rate(1/1.25); break;
        case sf::Keyboard::LShift: m_shift_pressed = true; break;
        case sf::Keyboard::C: m_black_points.clear(); m_white_points.clear(); break;
        default: break;
    }
}

void DotSeparator::on_key_release(sf::Keyboard::Key key) {
    switch(key) {
        case sf::Keyboard::LShift: m_shift_pressed = false; break;
        default: break;
    }
}

void DotSeparator::on_draw() {
    for(auto& point : m_black_points) {
        auto window_point = window_coordinates(point + Vec2d {0.5, 0.5});
        m_black_point.setPosition({window_point.x, window_point.y});
        m_window->draw(m_black_point);
    }

    for(auto& point : m_white_points) {
        auto window_point = window_coordinates(point + Vec2d {0.5, 0.5});
        m_white_point.setPosition({window_point.x, window_point.y});
        m_window->draw(m_white_point);
    }
}

void DotSeparator::on_mouse_press(double x, double y) {
    if(m_shift_pressed) m_black_points.emplace_back(x - 0.5, y - 0.5);
    else m_white_points.emplace_back(x - 0.5, y - 0.5);
}

void DotSeparator::on_tick() {
    learn();
    redraw();
}

void DotSeparator::adjust_learning_rate(double coef) {
    m_net->set_learning_rate(m_net->get_learning_rate() * coef);

    std::cout << "Adjusted learning rate to " << m_net->get_learning_rate() << "\n";
}
