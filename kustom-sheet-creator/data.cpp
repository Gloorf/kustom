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
#include <QtGui>
#include <QApplication>
#include <QDebug>
#include <QtWidgets>
#include "data.h"
#include "xml_reader.h"
#include "character.h"
CData::CData()
{
    /*
     * ID is XXYYYY
     * Where XX is a category of ID (like race, skill, perk ...), and YYYY is the ID in this category
     * For example, Human is ID 0000 inside race category, so its global ID is 010000,
     * Elf is 010001 ...
     * This allows to get 99 categories (more than enough), and 9999 items by category (more than enough, even for perk)
     * Misc : 0 is unknown skill
     * 1 is any weapon skill
     * 2 is any magic skill
     * 3 is any diverse skill
     * 4 !non weapon
     * 5 non magic
     * 6 non diverse
     * 7 any skill
*/
    _kustomIdType.insert("race", "01");
    _kustomIdType.insert("skill", "02");
    _kustomIdType.insert("perk", "03");
    _kustomIdType.insert("religion", "04");
    _kustomIdType.insert("religionPower", "05");
    _kustomIdType.insert("attribute", "06");
    _kustomIdType.insert("misc","99");
    _reader = new CXmlReader;

    createConst();
    _skillB=_reader->loadSkill();
    //damage initialisation
    _damageB = _reader->loadDamage();
    _bodyPartName = _reader->bodyPartName;
    _bodyZoneName = _reader ->bodyZoneName;
    //Race init
    _raceB=_reader->loadRace();
    //Religion init
    _religionB = _reader->loadReligion();

    _settings = new QSettings("data/kustom.ini", QSettings::IniFormat);
    //Race base stats & name

}


CData::~CData()
{
}

/*
 * Create data who doesn't need to be loaded (will never change, skill cost for example
 * */
void CData::createConst()
{
    //Carac name
    _caracName << "Pvs" << "Force" << "Agilité" << "Réflexes" << "Volonté" << "Mana";
    //Carac cost
    _caracCost << 15 << 10 << 25 << 25 << 30 << 10;
    _numCarac = _caracCost.size();
    //Skill point cost
    QList<qint32> costF, costM, costS;
    costF << 0 << 20 << 40 << 70 << 120 << 210 << 330 << 480 << 680 << 980 << 1430;
    costM << 0 << 30 << 55 << 95 << 155 << 285 << 450 << 650 << 925 << 1325 << 1875;
    costS << 0 << 50 << 80 << 130 << 205 << 380 << 630 << 930 << 1330 << 1830 << 2480;
    _skillCost.insert("f", costF);
    _skillCost.insert("m", costM);
    _skillCost.insert("s", costS);

    //Special ( talent + gift) cost
    _specialCost.insert("n", 0);
    _specialCost.insert("t", 70);
    _specialCost.insert("g", 170);
}


QList<CPerk *> CData::getUsablePerk(QString id, qint32 level)
{
    QList<CPerk *> output;
    CSkill * skill = getSkillById(id);
    for(int i=0;i<skill->getPerkB().size();i++)
    {
        if(skill->getPerkB()[i]->getParamInt("level")<=level)
        {
            output.append(skill->getPerkB()[i]);
        }
    }
    return output;
}


QStringList CData::getSkillNameList()
{
    QStringList output;
    for(int i=0;i<_skillB.size();i++)
    {
        output << _skillB[i]->getParam("name");
    }
    return output;
}

QStringList CData::getSkillIdList()
{
    QStringList output;
    for(int i=0;i<_skillB.size();i++)
    {
        output << _skillB[i]->getParam("id");
    }
    return output;
}

CPerk *CData::getDefaultPerkForSkill(QString skillId, qint32 level)
{
    for(int i=0;i<_skillB.size();i++)
    {
        CSkill *skill = _skillB[i];
        if(skill->getParam("id")==skillId)
        {
            for(int j=0;j<skill->getPerkB().size();j++)
            {
                CPerk *perk = skill->getPerkB()[j];
                if(perk->getParamInt("level") >= level)
                {
                    return perk;
                }
            }
        }
    }
    CPerk *perk = new CPerk;
    perk->setParam("id", "-1");
    return perk;

}

/*
 * All getters
 * */
QSettings *CData::getSettings() const
{
    return _settings;
}
QString CData::getFullId(QString type, qint32 local_id)
{
    QString output = _kustomIdType[type];
    //4 is because id are YYYY
    output += QString("0").repeated(4-QString::number(local_id).length());
    output += QString::number(local_id);
    return output;
}
qint32 CData::getNumCarac() const
{
    return _numCarac;
}
QStringList CData::getCaracName()
{
    return _caracName;
}

qint32 CData::getSkillCost(QString speed, qint32 level) const
{
    return _skillCost.value(speed)[level];
}
QList<qint32> CData::getCaracCost() const
{
    return _caracCost;
}
QMap<QString, qint32> CData::getSpecialCost() const
{
    return _specialCost;
}
QStringList CData::getBodyPartName() const
{
    return _bodyPartName;
}
QList<QStringList> CData::getBodyZoneName() const
{
    return _bodyZoneName;
}
QList<QList<QList<damageData> > > CData::getDamageB() const
{
    return _damageB;
}
QList<CSkill *> CData::getSkillB()
{
    return _skillB;
}

QList<CRace *> CData::getRaceB() const
{
    return _raceB;
}
QList<CReligion *> CData::getReligionB()
{
    return _religionB;
}
CSkill* CData::getSkillById(QString id)
{
    for (int i=0;i<_skillB.size();i++)
    {
        if(id == _skillB[i]->getParam("id"))
        {
            return _skillB[i];
        }
    }
    CSkill* output = new CSkill;
    output->setParam("id", "-1");
    return output;
}

CRace* CData::getRaceById(QString id)
{
    for(int i=0;i<_raceB.size();i++)
    {
        if(id == _raceB[i]->getId())
        {
            return _raceB[i];
        }
    }
    CRace* output = new CRace;
    output->setParam("id", "-1");
    return output;
}

CReligion* CData::getReligionById(QString id)
{
    for(int i=0;i<_religionB.size();i++)
    {
        if(id == _religionB[i]->getId())
        {
            return _religionB[i];
        }
    }
    CReligion* output = new CReligion;
    output->setParam("id", "-1");
    return output;
}

CPerk* CData::getPerkById(QString id)
{
    for(int i=0;i<_skillB.size();i++)
    {
        for(int j=0;j<_skillB[i]->getPerkB().size();j++)
        {
            if(id == _skillB[i]->getPerkB()[j]->getParamStr("id"))
            {
                return _skillB[i]->getPerkB()[j];
            }
        }
    }
    CPerk* output = new CPerk;
    output->setParam("id", "-1");
    output->setParam("cost", 0);
    return output;
}

religionPower CData::getReligionPowerById(QString id)
{
    for(int i=0;i<_religionB.size();i++)
    {
        //getPowerById return an id of -1 if nothing is found
        if(_religionB[i]->getPowerById(id).id !="-1")
        {
            return _religionB[i]->getPowerById(id);
        }
    }
    religionPower output;
    output.id="-1";
    return output;
}
