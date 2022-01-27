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

class DotSeparator : public GeneralApp {

    bool m_shift_pressed = false;
    double m_camera_scale = 1;

    std::vector<sf::Vector2<double>> m_white_points {};
    std::vector<sf::Vector2<double>> m_black_points {};

    sf::CircleShape m_white_point { 10 };
    sf::CircleShape m_black_point { 10 };

    std::unique_ptr<NeuralNetwork> m_net {};

public:
    DotSeparator();

    void learn();

    void redraw();

    void on_key_press(sf::Keyboard::Key key) override;

    void on_key_release(sf::Keyboard::Key key) override;

    void on_tick() override;

    void on_mouse_press(double x, double y) override;

    void on_draw() override;

    void adjust_learning_rate(double coef);
};