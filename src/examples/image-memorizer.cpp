//
// Created by Артем on 25.01.2022.
//

#include "image-memorizer.hpp"

ImageMemorizer::ImageMemorizer() : BitmapApp() {
    m_net = std::make_unique<NeuralNetwork>(2, 3, std::vector<int> {
            8, 8, 8, 8, 8, 8, 8, 8
    });

    m_net->randomize(0);

    m_target_image = std::make_unique<sf::Image>();
    m_target_image->loadFromFile("./resources/target.png");

    create_window(m_target_image->getSize().x, m_target_image->getSize().y);
}

void ImageMemorizer::learn() {
    auto& inputs = m_net->get_input();
    auto& outputs = m_net->get_output();
    auto& errors = m_net->get_errors();

    auto image_size = m_target_image->getSize();
    auto matrix_size = m_window_height * m_window_width;

    double loss = 0;

    for(int step = 0; step < m_tick_steps; step++) {
        int i = m_rng() % matrix_size;

        auto rel_x = (double)(i % m_window_width) / (double)m_window_width;
        auto rel_y = (double)(i / m_window_width) / (double)m_window_height;

        inputs[0] = rel_x * m_camera_scale - m_camera_scale / 2;
        inputs[1] = rel_y * m_camera_scale - m_camera_scale / 2;

        auto pixel = m_target_image->getPixel(rel_x * image_size.x, rel_y * image_size.y);

        m_net->evaluate<Sigmoid>();

        errors[0] = from_brightness(pixel.r) - outputs[0];
        errors[1] = from_brightness(pixel.g) - outputs[1];
        errors[2] = from_brightness(pixel.b) - outputs[2];

        loss += abs(errors[0]) + abs(errors[1]) + abs(errors[2]);

        m_net->descent<Sigmoid>();
    }

    std::cout << "learn: " << loss << "\n";
}

void ImageMemorizer::redraw() {
    std::vector<double>& inputs = m_net->get_input();
    const std::vector<double>& outputs = m_net->get_output();

    for(int i = 0; i < m_window_height * m_window_width; i++) {
        auto rel_x = (double)(i % m_window_width) / (double)m_window_width;
        auto rel_y = (double)(i / m_window_width) / (double)m_window_height;

        inputs[0] = rel_x * m_camera_scale - m_camera_scale / 2;
        inputs[1] = rel_y * m_camera_scale - m_camera_scale / 2;

        m_net->evaluate<Sigmoid>();

        m_pixels[i * 4 + 0] = to_brightness(outputs[0]);
        m_pixels[i * 4 + 1] = to_brightness(outputs[1]);
        m_pixels[i * 4 + 2] = to_brightness(outputs[2]);
        m_pixels[i * 4 + 3] = 255;
    }
}

void ImageMemorizer::on_key_press(sf::Keyboard::Key key) {
    switch(key) {
        case sf::Keyboard::Up:
            adjust_learning_rate(1.25);
            break;
        case sf::Keyboard::Down:
            adjust_learning_rate(1/1.25);
            break;
        case sf::Keyboard::P:
            redraw();
            break;
        default: break;
    }
}

void ImageMemorizer::on_tick() {
    learn();
}

void ImageMemorizer::adjust_learning_rate(double coef) {
    m_net->set_learning_rate(m_net->get_learning_rate() * coef);
}
