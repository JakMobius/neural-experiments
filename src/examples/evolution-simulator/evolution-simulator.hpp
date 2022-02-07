#pragma once

#include "../../general-app.hpp"
#include "../../graphics/scene-rendering/programs/scene-rendering-program.hpp"
#include "../../utils/vec2.hpp"
#include "user-controller.hpp"
#include "../../graphics/perspective_camera.hpp"
#include "world.hpp"
#include "objects/spring-object.hpp"
#include "objects/vertex-object.hpp"
#include "creatures/creature.hpp"
#include "creatures/creature-generator.hpp"
#include "creatures/creature-mutating-context.hpp"
#include <random>

struct ConfiguredCreature {
    Creature* m_creature;
    CreatureConfig m_config;

    bool operator<(const ConfiguredCreature& other) const {
        return m_creature->get_score() > other.m_creature->get_score();
    }
};

class EvolutionSimulator : public GeneralApp {

    UserController m_user_controller { (GeneralApp*) this };

    std::unique_ptr<EvolutionWorld> m_world {};
    std::unique_ptr<PerspectiveCamera> m_camera {};

    Graphics::GeometryObject* object = nullptr;

    CreatureGenerator m_creature_generator {};

    bool m_speedup = false;

    std::vector<ConfiguredCreature> m_creatures {};

    float m_generation_time = 0;

    int m_generation_index = 0;

    void create_window(int width, int height);

    void reset_gl();
    void clear_window();

public:

    EvolutionSimulator();

    void on_tick() override;

    void on_mouse_move(double x, double y) override;

    void on_draw() override;

    void on_key_press(sf::Keyboard::Key key) override;
    void on_key_release(sf::Keyboard::Key key) override;

    void create_initial_generation();
    void next_generation();
    void release_creatures();
};