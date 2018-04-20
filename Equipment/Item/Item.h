#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QMap>
#include <QVector>
#include <assert.h>

namespace ItemSlots {
    static const int MAINHAND = 0;
    static const int OFFHAND = 1;
    static const int RANGED = 2;
    static const int HEAD = 3;
    static const int NECK = 4;
    static const int SHOULDERS = 5;
    static const int BACK = 6;
    static const int CHEST = 7;
    static const int WRIST = 8;
    static const int GLOVES = 9;
    static const int BELT = 10;
    static const int LEGS = 11;
    static const int BOOTS = 12;
    static const int RING = 13;
    static const int TRINKET = 14;
    static const int CASTER_OFFHAND = 15;
    static const int RELIC = 16;
}

namespace WeaponTypes {
    static const int AXE = 0;
    static const int DAGGER = 1;
    static const int FIST = 2;
    static const int MACE = 3;
    static const int POLEARM = 4;
    static const int STAFF = 5;
    static const int SWORD = 6;
    static const int BOW = 7;
    static const int CROSSBOW = 8;
    static const int GUN = 9;
    static const int THROWN = 10;
    static const int WAND = 11;
}

namespace WeaponSlots {
    static const int ONEHAND = 0;
    static const int MAINHAND = 1;
    static const int OFFHAND = 2;
    static const int TWOHAND = 3;
    static const int RANGED = 4;
}

class Stats;

int get_slot_int(const QString& slot_string);

class Item {
public:
    Item(QString _name, QVector<QPair<QString, QString> > _stats, QMap<QString, QString> _info);
    virtual ~Item();

    virtual int get_item_slot(void) const;

    QString get_name(void) const;
    QString get_value(const QString& key) const;
    QString get_base_stat_tooltip() const;
    QString get_equip_effect_tooltip() const;

    const Stats* get_stats() const;
    void set_stat(const QString& key, const QString& value);
    void set_stats(QVector<QPair<QString, QString>> stats);


protected:
    QString name;
    QString patch;
    QString source;
    QString quality;
    QMap<QString, QString> info;
    QVector<QString> base_tooltip_stats;
    QVector<QString> equip_effects_tooltip_stats;
    Stats* stats;

    int slot;
    void set_item_slot(const QMap<QString, QString>& info);
    void unsupported_stat(const QString& stat);
    QString get_tooltip(const QVector<QString>&) const;

private:
};

#endif // ITEM_H
