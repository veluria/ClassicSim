#include "MageSpells.h"

#include "ClearcastingMage.h"
#include "Combustion.h"
#include "FireVulnerability.h"
#include "Fireball.h"
#include "Ignite.h"
#include "IgniteBuff.h"
#include "ImprovedScorch.h"
#include "Mage.h"
#include "MainhandAttack.h"
#include "RaidControl.h"
#include "Scorch.h"

MageSpells::MageSpells(Mage* mage) :
    CharacterSpells(mage),
    mage(mage)
{
    this->mh_attack = new MainhandAttack(mage);

    add_spell_group({mh_attack});

    add_spell_group({
                        new Fireball(mage, this, 1),
                        new Fireball(mage, this, 2),
                        new Fireball(mage, this, 3),
                        new Fireball(mage, this, 4),
                        new Fireball(mage, this, 5),
                        new Fireball(mage, this, 6),
                        new Fireball(mage, this, 7),
                        new Fireball(mage, this, 8),
                        new Fireball(mage, this, 9),
                        new Fireball(mage, this, 10),
                        new Fireball(mage, this, 11),
                        new Fireball(mage, this, 12),
                    });


    auto fire_vulnerability_buff = dynamic_cast<FireVulnerability*>(mage->get_raid_control()->get_shared_raid_buff("Fire Vulnerability"));
    if (fire_vulnerability_buff == nullptr) {
        fire_vulnerability_buff = new FireVulnerability(mage);
        fire_vulnerability_buff->enable_buff();
    }
    this->imp_scorch = new ImprovedScorch(mage, fire_vulnerability_buff);
    add_spell_group({
                        new Scorch(mage, this, imp_scorch, 1),
                        new Scorch(mage, this, imp_scorch, 2),
                        new Scorch(mage, this, imp_scorch, 3),
                        new Scorch(mage, this, imp_scorch, 4),
                        new Scorch(mage, this, imp_scorch, 5),
                        new Scorch(mage, this, imp_scorch, 6),
                        new Scorch(mage, this, imp_scorch, 7),
                    });

    auto* ignite_buff = dynamic_cast<IgniteBuff*>(mage->get_raid_control()->get_shared_raid_buff("Ignite"));
    if (ignite_buff == nullptr) {
        ignite_buff = new IgniteBuff(mage);
        ignite_buff->enable_buff();
    }
    ignite = new Ignite(mage, ignite_buff);
    add_spell_group({ignite});

    combustion = new Combustion(mage);
    add_spell_group({combustion});

    this->clearcasting = new ClearcastingMage(mage);
}

MageSpells::~MageSpells() {
    delete clearcasting;
    delete imp_scorch;
}

Combustion* MageSpells::get_combustion() const {
    return this->combustion;
}

Proc* MageSpells::get_clearcasting() const {
    return this->clearcasting;
}

Proc* MageSpells::get_improved_scorch() const {
    return this->imp_scorch;
}

void MageSpells::inflict_ignite(const double damage) {
    ignite->inflict_ignite(damage);
}

bool MageSpells::clearcasting_active() const {
    return clearcasting->buff->is_active();
}

void MageSpells::roll_clearcasting() {
    if (!clearcasting->is_enabled())
        return;

    clearcasting->buff->use_charge();
    if (clearcasting->check_proc_success())
        clearcasting->perform();
}
