#pragma once

#include <SFML/Window/Keyboard.hpp>
#include "../../utils/vec3.hpp"
#include "../../utils/vec2.hpp"
#include "../../general-app.hpp"
#include "../../graphics/camera.hpp"

class UserController {
    GeneralApp* app;
    Camera* m_controlled_camera;
    bool m_dummy_mouse_event_received = false;
    double pitch = 0;
    double yaw = 0;

    void move_mouse_to_center() {
        auto size = app->get_window()->getSize();
        sf::Mouse::setPosition((sf::Vector2i)size / 2, *app->get_window());
    }

public:

    UserController(GeneralApp* app): app(app) {

    }

    void set_controlled_camera(Camera* camera) {
        m_controlled_camera = camera;
    }

    static Vec3f get_motion_vector() {
        Vec3f result;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) result.z += 1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) result.x -= 1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) result.z -= 1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) result.x += 1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) result.y += 1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) result.y -= 1;

        return result;
    }

    void on_mouse_move(double x, double y) {
        auto size = app->get_window()->getSize();
        Vec2f center = {(float)size.x / 2, (float)size.y / 2};

        Vec2f position = {(float)x, (float)y};
        Vec2f delta = position - center;

        if(delta.len_squared() > 0) {
            pitch += delta.y / (float) size.y;
            yaw -= delta.x / (float) size.x;
            m_controlled_camera->set_pitch_yaw(pitch, yaw);
            move_mouse_to_center();
            app->get_window()->setMouseCursorVisible(false);
        } else {
            m_dummy_mouse_event_received = true;
        }
    }

    void on_tick() {

        Vec3f motion = UserController::get_motion_vector() * 0.01;
        motion *= Matrix4f::rotation_y_matrix(yaw);
        m_controlled_camera->get_position() += motion;

        if(app->get_window()->hasFocus() && !m_dummy_mouse_event_received) {
            move_mouse_to_center();
            m_dummy_mouse_event_received = false;
        }
    }
};