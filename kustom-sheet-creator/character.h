/*
Copyright (C) 2014 Guillaume DUPUY <glorf@glorf.fr>
This file is part of Kustom Sheet Creator.

Kustom Sheet Creator is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Kustom Sheet Creator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Kustom Sheet Creator.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef CHARACTER_H
#define CHARACTER_H
#include <QtGui>
#include <QApplication>
#include <QtWidgets>

#include <QDebug>
#include "data_type.h"
#include "data.h"

class CCharacter : public QWidget
{
    Q_OBJECT
public:
    CCharacter(CData *data);
    void updateRace(QString id);
    void addReligionPower(QString id);
    void removeReligionPower(QString id);

    void updateReligionEffect();
    void resetReligion();
    QList<religionPower> getReligionPowerList();
    void addSkill(QString id);
    void removeSkillFromIndex(qint32 skillIndex);
    void updateSkillFromIndex(qint32 skillIndex, QString newId);
    void setAttribute(QString name, qint32 value);
    qint32 getAttribute(QString name);

    void setName(QString name);
    void setAdvantagePoints(qint32 value);
    qint32 getPointsTotal();
    QString getRaceId();
    QString getName();
    QList<CUSkill *> getSkill();

    void setSkillParam(qint32 skillIndex, QString name, QString value);
    void updatePerkNumber(qint32 skillIndex, qint32 newNumber);
    qint32 getPerkNumberForSkill(qint32 skillIndex);
    void updatePerkFromIndex(qint32 skillIndex, qint32 perkIndex, QString newId);
    QString getRaceName();
    void updatePointsTotal(bool verbose=false);
private:
    CData *_d;
    QList<CUSkill *> _skills;
    CReligion *_religion;
    CRace *_race; //Character Race (BASE race)
    QMap<QString, qint32> _attributes;//Character attr
    QString _name;//Of the character
    qint32 _points;//Total point cost
    qint32 _advantage_points;
    /*
     * name, race
     * hp, strength, agility, reflexe, willpower, mana
     * point
     * religionHp/religionStrength
     *
     * */
    void updateRaceSkillSpeed(qint32 skillIndex);
    qint32 raceGiftPoints(bool verbose);
};


#endif // CHARACTER_H
