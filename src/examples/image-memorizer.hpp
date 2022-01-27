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

    double scale = 1;
    int tick_steps = 1000;

    std::mt19937 rng = {};

    std::unique_ptr<sf::Image> target_image {};
    std::unique_ptr<NeuralNetwork> net {};

public:
    ImageMemorizer();

    void learn();

    void redraw();

    void adjust_learning_rate(double coef);

    void on_key_press(sf::Keyboard::Key key) override;

    void on_tick() override;
};