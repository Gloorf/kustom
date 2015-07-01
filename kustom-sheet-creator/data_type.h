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

#ifndef DATA_TYPE_H
#define DATA_TYPE_H
#include <QtGui>
#include <QtWidgets>
#include <QApplication>
#include <QDebug>
struct charData
{
    QString name;
    QString race;
    qint32 hp;
    qint32 strength;
    qint32 agility;
    qint32 reflexe;
    qint32 willpower;
    qint32 mana;
    qint32 point;
    qint32 religionHp;
    qint32 religionStrength;
};



struct religionPower
{
    QString id;
    QString name;
    bool cult;
    qint32 cost;
    qint32 special;
};

/*For skull, we will have the following list of damageData :
 * {-1,1,0,TRUE} for 1 dé maxé (-1 mean we'll replace with the dice value)
 * {0,0,2,FALSE} for b*2
 * {-1,2,0,FALSE} for 2 dé arme (-1 same meaning avec dé maxé)
 * {10,2,0,FALSE} for 2d10
 * If multiple additionnal dice we just add another {}
 * */
struct damageData
{
    qint32 value;
    qint32 roll;
    qint32 bonus;
    bool max;
};
class CPerk
{
public:
    CPerk();
    qint32 getParamInt(QString name);
    QString getParamStr(QString name);
    bool getParamBool(QString name);
    void setParam(QString name, QString value);
    void setParam(QString name, qint32 value);
    void setPrerequisite(QString id, qint32 level = 0);
    QMap<QString, qint32> getPrerequisite();
    bool IsPrerequisitePerk();
    bool havePrerequisite();
private:
    QMap<QString, QString> _info;
    QMap<QString, qint32> _prerequisite;
    /* If we need 1 skill :
        *<prerequisite type="skill">
            <id>020037</id>
            <level>3</level>
        </prerequisite>
     * If we need 1 of 2 skills :
*    <prerequisite type="skill" number="2">
        <id1>020032</id1>
        <id2>020042</id2>
        <level>1</level>
    </prerequisite>
    If we need a perk :
    <prerequisite type="perk">
        <id>030009</id>
    </prerequisite>
     *Generic
     * QString id
    QString name;
    qint32 cost;
    qint32 level;
    QString desc;
    //Magic + weapon
    QString misc;
    QString cooldown;//String for big length
    //Weapon specific
    QString levelNeeded;//compName| level
    //Magic specific
    QString dd;
    bool slowIncant;
    bool fastIncant;
    bool focus;
    QString condition;//For monial only
    QString school;//Monial, psychic, elementary only
    QString optionalComponent;//Sorcery
    QString incantTime;//Sceau only
     * */
};
class CSkill
{
public:
    CSkill();
    void addPerk(CPerk *perk);
    CPerk *findPerk(QString name);
    QList<CPerk *> getPerkB();
    void removeAllPerk();
    bool isWeapon();
    bool isMagic();
    bool isMisc();
    bool multipleSchool();
    qint32 columnCount();
    qint32 rowCount();
    QStringList headerLabels();
    QStringList getSchools();
    void setParam(QString name, QString value);
    QString getParam(QString name);
private:
    QMap<QString, QString> _info;
    /*
     * Contains id, name, speed, type, special, level, schools
     * speed is f/m/s (fast/middle/slow)
     * type is w/m/d (weapon/magic/diverse)
     * special is n/t/g(none/talent/gift)
     * */

    //Actual whole perk list for one skill
protected:
    QList<CPerk *> _perkB;
};
class CUSkill : public CSkill
{
public:
    CUSkill(CSkill *base);
    qint32 getPerkNumber();
    void removeLastPerk();
    void updatePerkId(qint32 perkIndex, CPerk *newPerk);
    qint32 excessPerkNumber();
    void setSchool(QString school);
    QString getSchool();
private:
    QString _chosenSchool;

};


class CRace
{
public:
    CRace(QString id = "010000");
    void setParam(QString name, QString value);
    void setParam(QString name, qint32 value);
    qint32 getAttribute(QString name);
    void addGift(QString id, qint32 level);//Use only at class initialisation
    QString getName();
    QString getId();
    QMap<QString, qint32> _gift;

private:
    QString _id;
    QString _name;
    QMap<QString, qint32> _carac;//Also contains cost
};

class CReligion
{
public:
    CReligion();
    void addCompatibility(QString id, QString state); //Used at class initiliasiations
    void setParam(QString name, QString value);
    void addPower(religionPower power);
    void removePower(QString id);
    void removeAllPower();
    QString getId();
    QString getName();
    religionPower getPowerById(QString id);
    religionPower getCult();
    QList<religionPower> getPowerList();
    QMap<QString, QString> getCompatibilities();

private:
  QString _id;
  QString _name;
  QList<religionPower> _powers;
  QMap<QString, QString> _compatibilities;//<religion_ID, state>
};

class CVerticalScrollArea : public QScrollArea
{
    Q_OBJECT
    public:
    explicit CVerticalScrollArea(QWidget *widget);

    virtual bool eventFilter(QObject *o, QEvent *e);
};
#endif // DATA_TYPE_H
