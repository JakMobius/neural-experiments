//
// Created by Артем on 25.01.2022.
//

#include "image-memorizer.hpp"

ImageMemorizer::ImageMemorizer() : GeneralApp() {
    net = std::make_unique<NeuralNetwork>(2, 3, std::vector<int> {
            8, 8, 8, 8, 8, 8, 8, 8
    });

    target_image = std::make_unique<sf::Image>();
    target_image->loadFromFile("./resources/target.png");

    create_window(target_image->getSize().x, target_image->getSize().y);
}

void ImageMemorizer::learn() {
    auto& inputs = net->get_input();
    auto& outputs = net->get_output();
    auto& errors = net->get_errors();

    auto image_size = target_image->getSize();
    auto matrix_size = m_window_height * m_window_width;

    double loss = 0;

    for(int step = 0; step < tick_steps; step++) {
        int i = rng() % matrix_size;

        auto rel_x = (double)(i % m_window_width) / (double)m_window_width;
        auto rel_y = (double)(i / m_window_width) / (double)m_window_height;

        inputs[0] = rel_x * scale - scale / 2;
        inputs[1] = rel_y * scale - scale / 2;

        auto pixel = target_image->getPixel(rel_x * image_size.x, rel_y * image_size.y);

        net->evaluate<Sigmoid>();

        errors[0] = from_brightness(pixel.r) - outputs[0];
        errors[1] = from_brightness(pixel.g) - outputs[1];
        errors[2] = from_brightness(pixel.b) - outputs[2];

        loss += abs(errors[0]) + abs(errors[1]) + abs(errors[2]);

        net->descent<Sigmoid>();
    }

    std::cout << "learn: " << loss << "\n";
}

void ImageMemorizer::redraw() {
    std::vector<double>& inputs = net->get_input();
    const std::vector<double>& outputs = net->get_output();

    for(int i = 0; i < m_window_height * m_window_width; i++) {
        auto rel_x = (double)(i % m_window_width) / (double)m_window_width;
        auto rel_y = (double)(i / m_window_width) / (double)m_window_height;

        inputs[0] = rel_x * scale - scale / 2;
        inputs[1] = rel_y * scale - scale / 2;

        net->evaluate<Sigmoid>();

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
    net->set_learning_rate(net->get_learning_rate() * coef);
}
