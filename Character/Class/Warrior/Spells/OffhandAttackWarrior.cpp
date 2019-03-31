#include "OffhandAttackWarrior.h"

#include "CharacterStats.h"
#include "CombatRoll.h"
#include "DeepWounds.h"
#include "OverpowerBuff.h"
#include "RecklessnessBuff.h"
#include "Warrior.h"

OffhandAttackWarrior::OffhandAttackWarrior(Character* pchar) :
    OffhandAttack(pchar),
    TalentRequirer(QVector<TalentRequirerInfo*>{new TalentRequirerInfo("Dual Wield Specialization", 5, DisabledAtZero::No)}),
    warr(dynamic_cast<Warrior*>(pchar))
{
    talent_ranks = {0.5, 0.525, 0.55, 0.575, 0.6, 0.625};
    offhand_penalty = talent_ranks[0];
}

void OffhandAttackWarrior::increase_talent_rank_effect(const QString&, const int curr) {
    offhand_penalty = talent_ranks[curr];
}

void OffhandAttackWarrior::decrease_talent_rank_effect(const QString&, const int curr) {
    offhand_penalty = talent_ranks[curr];
}

void OffhandAttackWarrior::extra_attack() {
    calculate_damage(false);
}

void OffhandAttackWarrior::spell_effect() {
    complete_swing();
    calculate_damage(true);
}

void OffhandAttackWarrior::calculate_damage(const bool run_procs) {
    const int oh_wpn_skill = warr->get_oh_wpn_skill();
    int result = roll->get_melee_hit_result(oh_wpn_skill, pchar->get_stats()->get_oh_crit_chance());

    if (result == PhysicalAttackResult::MISS) {
        increment_miss();
        return;
    }

    if (result == PhysicalAttackResult::DODGE) {
        increment_dodge();
        warr->get_overpower_buff()->apply_buff();
        warr->gain_rage(warr->rage_gained_from_dd(warr->get_avg_mh_damage()));
        return;
    }
    if (result == PhysicalAttackResult::PARRY) {
        increment_parry();
        warr->gain_rage(warr->rage_gained_from_dd(warr->get_avg_mh_damage()));
        return;
    }
    if (result == PhysicalAttackResult::BLOCK || result == PhysicalAttackResult::BLOCK_CRITICAL) {
        increment_full_block();
        warr->gain_rage(warr->rage_gained_from_dd(warr->get_avg_mh_damage()));
        return;
    }

    if (warr->get_recklessness_buff()->is_active())
        result = PhysicalAttackResult::CRITICAL;

    double damage_dealt = damage_after_modifiers(warr->get_random_non_normalized_oh_dmg() * offhand_penalty);

    if (result == PhysicalAttackResult::CRITICAL) {
        damage_dealt = round(damage_dealt * 2);
        add_crit_dmg(static_cast<int>(round(damage_dealt)), resource_cost, 0);
        const unsigned rage_gained = warr->rage_gained_from_dd(static_cast<unsigned>(round(damage_dealt)));
        // TODO: Save statistics for resource gains
        warr->gain_rage(rage_gained);

        warr->melee_oh_white_critical_effect(run_procs);
        return;
    }

    warr->melee_oh_white_hit_effect(run_procs);

    if (result == PhysicalAttackResult::GLANCING) {
        damage_dealt = round(damage_dealt * roll->get_glancing_blow_dmg_penalty(oh_wpn_skill));
        add_glancing_dmg(static_cast<int>(damage_dealt), resource_cost, 0);
        const unsigned rage_gained = warr->rage_gained_from_dd(static_cast<unsigned>(damage_dealt));
        // TODO: Save statistics for resource gains
        warr->gain_rage(rage_gained);
        return;
    }

    damage_dealt = round(damage_dealt);
    add_hit_dmg(static_cast<int>(damage_dealt), resource_cost, 0);
    const unsigned rage_gained = warr->rage_gained_from_dd(static_cast<unsigned>(damage_dealt));
    // TODO: Save statistics for resource gains
    warr->gain_rage(rage_gained);
}
