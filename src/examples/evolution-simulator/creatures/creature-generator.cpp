//
// Created by Артем on 04.02.2022.
//

#include "creature-generator.hpp"
#include "creature-creating-context.hpp"
#include "creature-mutating-context.hpp"

void CreatureGenerator::generate_creature(CreatureConfig* target) {
    CreatureCreatingContext ctx { this, target };
    ctx.generate_creature();
}

void CreatureGenerator::mutate_creature( CreatureConfig* target) {
    CreatureMutatingContext ctx { this, target };
    ctx.mutate_creature();
}
