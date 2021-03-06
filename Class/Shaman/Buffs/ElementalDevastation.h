#pragma once

#include "SelfBuff.h"
#include "TalentRequirer.h"

#include <QVector>

class ElementalDevastation: public SelfBuff, public TalentRequirer {
public:
    ElementalDevastation(Character* pchar);

private:
    unsigned crit_bonus = {0};
    QVector<unsigned> crit_bonus_ranks = {0, 300, 600, 900};

    void buff_effect_when_applied() override;
    void buff_effect_when_removed() override;

    void increase_talent_rank_effect(const QString& talent_name, const int curr) override;
    void decrease_talent_rank_effect(const QString& talent_name, const int curr) override;
};
