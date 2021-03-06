#pragma once

#include "TestSpellRogue.h"

class TestSliceAndDice: public TestSpellRogue {
public:
    TestSliceAndDice(EquipmentDb *equipment_db);

    void test_all();

private:
    SliceAndDice* slice_and_dice() const;

    void test_name_correct() override;
    void test_spell_cooldown() override;
    void test_whether_spell_causes_global_cooldown() override;
    void test_how_spell_observes_global_cooldown() override;
    void test_resource_cost() override;
    void test_is_ready_conditions() override;
    void test_combo_points() override;
    void test_stealth() override;

    void test_duration_and_attack_speed_with_1_combo_points();
    void test_duration_and_attack_speed_with_2_combo_points();
    void test_duration_and_attack_speed_with_3_combo_points();
    void test_duration_and_attack_speed_with_4_combo_points();
    void test_duration_and_attack_speed_with_5_combo_points();

    void test_duration_and_attack_speed_with_5_combo_points_and_1_of_3_imp_snd();
    void test_duration_and_attack_speed_with_5_combo_points_and_2_of_3_imp_snd();
    void test_duration_and_attack_speed_with_5_combo_points_and_3_of_3_imp_snd();

    void when_slice_and_dice_is_performed();
};
