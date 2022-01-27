#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>

class GeneralApp {

private:
    void process_event(const sf::Event &event);

protected:
    unsigned int m_window_width = 0;
    unsigned int m_window_height = 0;
    int m_scale = 1;

    std::unique_ptr<sf::RenderWindow> m_window {};
    std::unique_ptr<sf::Uint8[]> m_pixels {};

    void create_window(unsigned int width, unsigned int height, int scale = 1);

public:

    GeneralApp() {}

    virtual void on_key_press(sf::Keyboard::Key key) {}
    virtual void on_key_release(sf::Keyboard::Key key) {}
    virtual void on_tick() {}
    virtual void on_draw() {}
    virtual void on_mouse_press(double x, double y) {}

    sf::Vector2<float> window_coordinates(const sf::Vector2<double>& space_coordinates);
    sf::Vector2<double> space_coordinates(const sf::Vector2<float>& window_coordinates);

    void loop();
};