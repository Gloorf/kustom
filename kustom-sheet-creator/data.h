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

#ifndef DATA_H
#define DATA_H
#include <QtGui>
#include <QApplication>
#include <QDebug>
#include <QtWidgets>

#include "xml_reader.h"
#include "data_type.h"
class CData : public QObject
{
    Q_OBJECT
public:
    CData();
    ~CData();
    QSettings *getSettings() const;
    QString getFullId(QString type, qint32 local_id);//See kustomIdType

    qint32 getNumCarac() const;
    QStringList getCaracName();
    qint32 getSkillCost(QString speed, qint32 level) const;
    QList<qint32> getCaracCost() const;
    QMap<QString, qint32> getSpecialCost() const;

    QStringList getBodyPartName() const;
    QList<QStringList> getBodyZoneName() const;
    //Getter for whole datasets
    QList<QList<QList<damageData> > > getDamageB() const;
    QList<CSkill *> getSkillB();
    QList<CRace *> getRaceB() const;
    QList<CReligion *> getReligionB();
    //Getter for a single data
    CSkill *getSkillById(QString id);
    CRace *getRaceById(QString id);
    CReligion *getReligionById(QString id);
    CPerk *getPerkById(QString id);
    religionPower getReligionPowerById(QString id);

    QList<CPerk *> getUsablePerk(QString id, qint32 level);
    QStringList getSkillNameList();



    QStringList getSkillIdList();
    CPerk *getDefaultPerkForSkill(QString skillId, qint32 level);
private:
    void createConst();

    QMap<QString, QString> _kustomIdType;//Could probably be in a better place

    QSettings *_settings;
    CXmlReader *_reader;
    //I don't want to hardcode that
    qint32 _numCarac;
    QStringList _caracName;
    QMap<QString, QList<qint32> > _skillCost;
    QList<qint32> _caracCost;
    QMap<QString, qint32> _specialCost;

    //Interesting data
    QList<CRace *> _raceB;
    QList<CSkill *> _skillB;
    QList<CReligion *> _religionB;
    QStringList _bodyPartName;
    QList<QStringList> _bodyZoneName;
    QList<QList<QList<damageData> > > _damageB;


    //What is damageB like
    /* We have something like
     * {
     * [0-Head]= { [0-Skull]= {-1,1,0,true}, {0, 0, 2, FALSE}, {-1, 2, 0, FALSE}
     *             [1-Face] = damageData (too lazy to copy them) too
     *           }
     * [1-Stomach] = { [0-stomach] = damageData
     *                 [1-rib]
     *                  ...
     *                }
     * ...
     * }
     *
     * */
};
#endif // DATA_H
