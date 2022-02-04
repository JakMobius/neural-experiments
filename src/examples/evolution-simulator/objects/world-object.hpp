#pragma once

class EvolutionWorld;

class WorldObject {
protected:
    EvolutionWorld* m_world;
public:
    WorldObject(EvolutionWorld* world): m_world(world) {}
    virtual ~WorldObject() {}

    virtual void tick(float dt) {}
};