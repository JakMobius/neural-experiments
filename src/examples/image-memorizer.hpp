#pragma once

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "../perceptron/neural-network.hpp"
#include "../activators/sigmoid.hpp"
#include "../general-app.hpp"
#include "../utils.hpp"

class ImageMemorizer : public GeneralApp {

    int m_tick_steps = 1000;
    double m_camera_scale = 1;

    std::mt19937 m_rng = {};

    std::unique_ptr<sf::Image> m_target_image {};
    std::unique_ptr<NeuralNetwork> m_net {};

public:
    ImageMemorizer();

    void learn();

    void redraw();

    void adjust_learning_rate(double coef);

    void on_key_press(sf::Keyboard::Key key) override;

    void on_tick() override;
};