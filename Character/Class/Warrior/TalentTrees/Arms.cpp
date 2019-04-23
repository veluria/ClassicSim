#include "Arms.h"

#include <QVector>

#include "AngerManagement.h"
#include "AxeSpecialization.h"
#include "DeepWounds.h"
#include "HeroicStrike.h"
#include "Impale.h"
#include "MortalStrike.h"
#include "Overpower.h"
#include "PolearmSpecialization.h"
#include "Rend.h"
#include "SwordSpecializationTalent.h"
#include "TacticalMastery.h"
#include "Talent.h"
#include "TwoHandedWeaponSpecialization.h"
#include "Warrior.h"
#include "WarriorSpells.h"

Arms::Arms(Warrior* pchar) :
    TalentTree("Arms", "Assets/warrior/warrior_arms.jpg"),
    warr(pchar),
    spells(dynamic_cast<WarriorSpells*>(pchar->get_spells()))
{
    QMap<QString, Talent*> tier1 {{"1LL", get_improved_heroic_strike()},
                                  {"1ML", new Talent(pchar, this, "Deflection", "1ML", base_url + "ability/Ability_parry.png", 5, "Increase your Parry chance by %1%.", QVector<QPair<unsigned, unsigned>>{{1, 1}})},
                                  {"1MR", get_improved_rend()}};
    add_talents(tier1);

    QMap<QString, Talent*> tier2 {{"2LL", new Talent(pchar, this, "Improved Charge", "2LL", base_url + "ability/Ability_warrior_charge.png", 2, "Increases the amount of rage generated by your Charge ability by %1.", QVector<QPair<unsigned, unsigned>>{{3, 2}})},
                                  {"2ML", new TacticalMastery(pchar, this)},
                                  {"2RR", new Talent(pchar, this, "Improved Thunder Clap", "2RR", base_url + "spell/Spell_nature_thunderclap.png", 3, "Reduces the cost of your Thunder Clap ability by %1 rage.", QVector<QPair<unsigned, unsigned>>{{1, 1}})}};
    add_talents(tier2);

    QMap<QString, Talent*> tier3 {{"3LL", get_improved_overpower()},
                                  {"3ML", get_anger_management()},
                                  {"3MR", get_deep_wounds()}};
    add_talents(tier3);

    QMap<QString, Talent*> tier4 {{"4ML", new TwoHandedWeaponSpecialization(pchar, this)},
                                  {"4MR", new Impale(pchar, this)}};
    add_talents(tier4);

    QMap<QString, Talent*> tier5 {{"5LL", new AxeSpecialization(pchar, this)},
                                  {"5ML", new Talent(pchar, this, "Sweeping Strikes", "5ML", "Assets/ability/Ability_rogue_slicedice.png", 1, "Your next 5 melee attacks strike an additional nearby opponent.", QVector<QPair<unsigned, unsigned>>{})},
                                  {"5MR", new Talent(pchar, this, "Mace Specialization", "5MR", "Assets/items/Inv_mace_01.png", 5, "Gives you a %1% chance to stun your target for 3 sec with a Mace.", QVector<QPair<unsigned, unsigned>>{{1, 1}})},
                                  {"5RR", new SwordSpecializationTalent(pchar, this)}};
    add_talents(tier5);

    QMap<QString, Talent*> tier6 {{"6LL", new PolearmSpecialization(pchar, this)},
                                  {"6MR", new Talent(pchar, this, "Improved Hamstring", "6MR", base_url + "ability/Ability_shockwave.png", 3, "Gives your Hamstring ability a %1% chance to immobilize the target for 5 sec.", QVector<QPair<unsigned, unsigned>>{{5, 5}})}};
    add_talents(tier6);

    QMap<QString, Talent*> tier7 {{"7ML", get_mortal_strike()}};
    add_talents(tier7);

    talents["1MR"]->talent->set_bottom_child(talents["3MR"]->talent);
    talents["3MR"]->talent->set_parent(talents["1MR"]->talent);

    talents["3MR"]->talent->set_bottom_child(talents["4MR"]->talent);
    talents["4MR"]->talent->set_parent(talents["3MR"]->talent);

    talents["2ML"]->talent->set_bottom_child(talents["3ML"]->talent);
    talents["3ML"]->talent->set_parent(talents["2ML"]->talent);

    talents["5ML"]->talent->set_bottom_child(talents["7ML"]->talent);
    talents["7ML"]->talent->set_parent(talents["5ML"]->talent);
}

Arms::~Arms() = default;

Talent* Arms::get_improved_heroic_strike() {
    QMap<unsigned, QString> rank_descriptions;
    Talent::initialize_rank_descriptions(rank_descriptions,
                                         "Reduces the cost of your Heroic Strike ability by %1 rage.",
                                         3,
                                         QVector<QPair<unsigned, unsigned>>{{1, 1}});
    Talent* talent = new Talent(warr, this, "Improved Heroic Strike", "1LL",
                                "Assets/ability/Ability_rogue_ambush.png", 3, rank_descriptions,
                                QVector<Spell*>{spells->get_heroic_strike()});

    return talent;
}

Talent* Arms::get_improved_rend() {
    QMap<unsigned, QString> rank_descriptions;
    Talent::initialize_rank_descriptions(rank_descriptions,
                                         "Increases the bleed damage done by your Rend ability by %1%.",
                                         3,
                                         QVector<QPair<unsigned, unsigned>>{{15, 10}});
    Talent* talent = new Talent(warr, this, "Improved Rend", "1MR",
                                "Assets/ability/Ability_gouge.png", 3, rank_descriptions,
                                QVector<Spell*>{spells->get_rend()});

    return talent;
}

Talent* Arms::get_improved_overpower() {
    QMap<unsigned, QString> rank_descriptions;
    Talent::initialize_rank_descriptions(rank_descriptions,
                                         "Increase the critical strike chance of your Overpower ability by %1%.",
                                         2,
                                         QVector<QPair<unsigned, unsigned>>{{25, 25}});
    Talent* talent = new Talent(warr, this, "Improved Overpower", "3LL",
                                "Assets/items/Inv_sword_05.png", 2, rank_descriptions,
                                QVector<Spell*>{spells->get_overpower()});

    return talent;
}

Talent* Arms::get_anger_management() {
    QMap<unsigned, QString> rank_descriptions;
    QString base_str = "Increases the time required for your rage to decay while out of combat by 30%.";
    rank_descriptions.insert(0, base_str);
    rank_descriptions.insert(1, base_str);
    Talent* talent = new Talent(warr, this, "Anger Management", "3ML",
                                "Assets/spell/Spell_holy_blessingofstamina.png", 1, rank_descriptions,
                                QVector<Spell*>{spells->get_anger_management()});

    return talent;
}

Talent* Arms::get_deep_wounds() {
    QMap<unsigned, QString> rank_descriptions;
    Talent::initialize_rank_descriptions(rank_descriptions,
                                         "Your critical strikes cause the opponent to bleed, dealing %1% of your melee weapon's average damage over 12 sec.",
                                         3,
                                         QVector<QPair<unsigned, unsigned>>{{20, 20}});
    Talent* talent = new Talent(warr, this, "Deep Wounds", "3MR",
                                "Assets/ability/Ability_backstab.png", 3, rank_descriptions,
                                QVector<Spell*>{spells->get_deep_wounds()});

    return talent;
}

Talent* Arms::get_mortal_strike() {
    QMap<unsigned, QString> rank_descriptions;
    QString base_str = "A vicious strike that deals weapon damage plus 85 and wounds the target, reducing the effectiveness of any healing by 50% for 10 sec.";
    rank_descriptions.insert(0, base_str);
    rank_descriptions.insert(1, base_str);
    Talent* talent = new Talent(warr, this, "Mortal Strike", "7ML",
                                "Assets/ability/Ability_warrior_savageblow.png", 1, rank_descriptions,
                                QVector<Spell*>{spells->get_mortal_strike()});

    return talent;
}
