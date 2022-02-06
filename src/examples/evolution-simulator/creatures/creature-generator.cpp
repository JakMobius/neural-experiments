//
// Created by Артем on 04.02.2022.
//

#include "creature-generator.hpp"
#include "creature-creating-context.hpp"

void CreatureGenerator::generate_creature(int seed, CreatureConfig* target) {
    m_rnd.seed(seed);
    CreatureCreatingContext ctx { this, target };
    ctx.generate_creature();
}
