#pragma once

#include "general-app.hpp"
#include "utils/vec2.hpp"

class BitmapApp : public GeneralApp {

protected:
    std::unique_ptr<sf::Uint8[]> m_pixels {};
    int m_scale = 1;

    unsigned int m_window_width = 0;
    unsigned int m_window_height = 0;

    sf::Texture m_texture {};
    sf::Sprite m_sprite {};

public:
    BitmapApp() {

    }

    void create_window(unsigned int width, unsigned int height, int scale = 1);

    Vec2f window_coordinates(const Vec2d& space_coordinates) const;
    Vec2d space_coordinates(const Vec2f& window_coordinates) const;

    void on_tick() override;

};