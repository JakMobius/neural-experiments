//
// Created by Артем on 25.01.2022.
//

#include "dot-separator.hpp"
#include "../utils.hpp"

DotSeparator::DotSeparator() : GeneralApp() {
    create_window(200, 200, 4);

    net = std::make_unique<NeuralNetwork>(2, 1, std::vector<int> {
        16, 16, 16, 16
    });

    net->set_learning_rate(0.01);

    for(int i = 0; i < 10; i++) {
        white_points.emplace_back(-0.5 + 0.1 * i, 0.1 + sin((double)i * 0.7) * 0.15);
        black_points.emplace_back(-0.5 + 0.1 * i, sin((double)i * 0.7) * 0.15);
        white_points.emplace_back(-0.5 + 0.1 * i, -0.1 + sin((double)i * 0.7) * 0.15);
    }

    white_point.setFillColor({255, 255, 255, 255});
    black_point.setFillColor({0, 0, 0, 255});
    white_point.setOutlineColor({0, 255, 0, 255});
    black_point.setOutlineColor({0, 255, 0, 255});
    white_point.setOutlineThickness(2);
    black_point.setOutlineThickness(2);

    white_point.setOrigin(10, 10);
    black_point.setOrigin(10, 10);
}

void DotSeparator::learn() {
    auto& inputs = net->get_input();
    auto& outputs = net->get_output();
    auto& errors = net->get_errors();

    for(int i = 0; i < 1000; i++) {

        for(auto& point : black_points) {
            inputs[0] = point.x;
            inputs[1] = point.y;

            net->evaluate<Sigmoid>();
            errors[0] = 0.0 - outputs[0];
            net->descent<Sigmoid>();
        }

        for(auto& point : white_points) {
            inputs[0] = point.x;
            inputs[1] = point.y;

            net->evaluate<Sigmoid>();
            errors[0] = 1.0 - outputs[0];
            net->descent<Sigmoid>();
        }
    }
}

void DotSeparator::redraw() {
    auto& inputs = net->get_input();
    auto& outputs = net->get_output();

    for(int i = 0; i < m_window_height * m_window_width; i++) {
        auto rel_x = (double)(i % m_window_width) / (double)m_window_width;
        auto rel_y = (double)(i / m_window_width) / (double)m_window_height;

        inputs[0] = (rel_x - 0.5) * scale;
        inputs[1] = (rel_y - 0.5) * scale;

        net->evaluate<Sigmoid>();

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
        case sf::Keyboard::LShift: shift_pressed = true; break;
        case sf::Keyboard::C: black_points.clear(); white_points.clear(); break;
        default: break;
    }
}

void DotSeparator::on_key_release(sf::Keyboard::Key key) {
    switch(key) {
        case sf::Keyboard::LShift: shift_pressed = false; break;
        default: break;
    }
}

void DotSeparator::on_draw() {
    for(auto& point : black_points) {
        black_point.setPosition(window_coordinates(point + sf::Vector2<double> {0.5, 0.5}));
        m_window->draw(black_point);
    }

    for(auto& point : white_points) {
        white_point.setPosition(window_coordinates(point + sf::Vector2<double> {0.5, 0.5}));
        m_window->draw(white_point);
    }
}

void DotSeparator::on_mouse_press(double x, double y) {
    if(shift_pressed) black_points.emplace_back(x - 0.5, y - 0.5);
    else white_points.emplace_back(x - 0.5, y - 0.5);
}

void DotSeparator::on_tick() {
    learn();
    redraw();
}

void DotSeparator::adjust_learning_rate(double coef) {
    net->set_learning_rate(net->get_learning_rate() * coef);

    std::cout << "Adjusted learning rate to " << net->get_learning_rate() << "\n";
}
