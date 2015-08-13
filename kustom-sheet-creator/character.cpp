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
#include <QtWidgets>
#include <QApplication>
#include <QDebug>
#include "character.h"

CCharacter::CCharacter(CData *data)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    _d = data;
    _religion = new CReligion;
    _race = new CRace("-1");//Easier to do it this way
    updateRace(_d->getFullId("race",0));
    _name="";
    _points=0;
    _advantage_points=0;
    _attributes["strengthReligion"] = 0;
    _attributes["healthReligion"] = 0;
    _attribute_name << "health" << "strength"<< "agility"<< "reflexe"<< "willpower" << "mana";
}
qint32 CCharacter::randInt(qint32 low, qint32 high)
{
    return qrand() % ((high + 1) - low) + low;
}
void CCharacter::randomise()
{
    qint32 rdm_id = randInt(0, _d->getRaceB().size() -1);
    updateRace(_d->getFullId("race", rdm_id));
    _attributes["health"] += randInt(5, 15);
    _attributes["strength"] += randInt(5, 15);
    _attributes["reflexe"] += randInt(5, 15);
    _attributes["agility"] += randInt(5, 15);
    _attributes["willpower"] += randInt(5, 15);
    _attributes["mana"] += randInt(5, 15);
    _advantage_points += randInt(7, 15);
    qint32 rdm_skill_id = randInt(0, _d->getSkillB().size() -1);
    addSkill(_d->getFullId("skill", rdm_skill_id));
    setSkillParam(0, "level", QString::number(randInt(1,5)));
    QStringList spec;
    spec << "n" << "t" << "g";
    setSkillParam(0, "special", spec[randInt(0,2)]);
    updatePerkNumber(0, getPerkNumberForSkill(0));
}

void CCharacter::updateRace(QString id)
{
    if(id != _race->getId())//Don't wanna lose our kustom stats
    {
        _race = _d->getRaceById(id);
        _attributes["health"]=_race->getAttribute("health");
        _attributes["strength"]=_race->getAttribute("strength");
        _attributes["reflexe"]=_race->getAttribute("reflexe");
        _attributes["agility"]=_race->getAttribute("agility");
        _attributes["willpower"]=_race->getAttribute("willpower");
        _attributes["mana"]=_race->getAttribute("mana");
        //Make sure we have the correct speed for our race
        for(int i=0;i<_skills.size();i++)
        {
            updateRaceSkillSpeed(i);
        }

    }
}

void CCharacter::updateReligionEffect()
{
    _attributes["strengthReligion"] = 0;
    _attributes["healthReligion"] = 0;
    for(int i=0;i<_religion->getPowerList().size();i++)
    {
        religionPower power = _religion->getPowerList()[i];
        switch(power.special)
        {
            case 11://Prières de complexion
                _attributes["healthReligion"] += 10;
            break;
            case 12://Membru de Chyrma
                _attributes["healthReligion"] += 10;
                _attributes["strengthReligion"] += 4;
            break;
            case 13://Élu des Dieux
                _attributes["healthReligion"] += 25;
            break;
            case 84://Force de la Naturere
                _attributes["healthReligion"] += 25;
                _attributes["strengthReligion"] += 20;
            break;
        default:
            break;

        }
    }
}

qint32 CCharacter::getReligionPoints()
{
    qint32 output = 0;
    for(int i=0;i<_religion->getPowerList().size();i++)
    {
        output += _religion->getPowerList()[i].cost;
    }
    return output;
}

void CCharacter::addReligionPower(QString id)
{
    _religion->addPower(_d->getReligionPowerById(id));
    updateReligionEffect();
}

void CCharacter::removeReligionPower(QString id)
{
    _religion->removePower(id);
    updateReligionEffect();
}

void CCharacter::resetReligion()
{
    _religion->removeAllPower();
    updateReligionEffect();
}

QList<religionPower> CCharacter::getReligionPowerList()
{
    return _religion->getPowerList();
}

void CCharacter::addSkill(QString id)
{
    _skills << new CUSkill(_d->getSkillById(id));
    updateRaceSkillSpeed(_skills.size()-1);//Make sure correct speed for the last skill
}

void CCharacter::removeSkillFromIndex(qint32 skillIndex)
{
    _skills.removeAt(skillIndex);
}

void CCharacter::updateSkillFromIndex(qint32 skillIndex, QString newId)
{
    if(newId != _skills[skillIndex]->getParam("id"))
    {
        _skills[skillIndex] = new CUSkill(_d->getSkillById(newId));
    }
}

void CCharacter::updatePerkFromIndex(qint32 skillIndex, qint32 perkIndex, QString newId)
{
    if(newId != _skills[skillIndex]->getPerkB()[perkIndex]->getParamStr("id"))
    {
        CPerk *perk = _d->getPerkById(newId);
        _skills[skillIndex]->updatePerkId(perkIndex, perk);
    }
}

void CCharacter::setAttribute(QString name, qint32 value)
{
    _attributes.insert(name, value);
}

void CCharacter::setAttributeById(qint32 index, qint32 value)
{

    _attributes.insert(_attribute_name[index], value);
}

qint32 CCharacter::getAttribute(QString name)
{
    return _attributes.value(name);
}

qint32 CCharacter::getAttributeById(qint32 index)
{
    return _attributes.value(_attribute_name[index]);
}

QString CCharacter::getRaceId()
{
    return _race->getId();
}

void CCharacter::setAdvantagePoints(qint32 value)
{
    _advantage_points = value;
}
qint32 CCharacter::getAdvantagePoints()
{
    return _advantage_points;
}

qint32 CCharacter::raceGiftPoints(bool verbose)
{
    qint32 output = 0;
    QStringList giftId = _race->_gift.keys();
    for(int i=0;i<giftId.size();i++)
    {
        qint32 tmp = 0;
        for(int j=0;j<_skills.size();j++)
        {
            qint32 tmp2 = 0;
            if(         giftId[i] == _skills[j]->getParam("id")
                    || (giftId[i] == "990001" && _skills[j]->isWeapon() )
                    || (giftId[i] == "990002" && _skills[j]->isMagic() )
                    || (giftId[i] == "990003" && _skills[j]->isMisc() )
                    || (giftId[i] == "990004" && !_skills[j]->isWeapon() )
                    || (giftId[i] == "990005" && !_skills[j]->isMagic() )
                    || (giftId[i] == "990006" && !_skills[j]->isMisc() )
                    ||  giftId[i] == "990007" )
            {
                tmp2 = _d->getSkillCost(_skills[j]->getParam("speed"), _race->_gift.value(giftId[i]));
            }
            if(tmp2 > tmp)
            {
                tmp = tmp2;
            }
        }
        if(tmp==0 && verbose)
        {
            QString message="Vous avez le droit à une compétence ";
            if(giftId[i] == "990001")
            {
                message += "martiale ";
            }
            if(giftId[i] == "990002")
            {
                message +="magique ";
            }
            if(giftId[i] == "990003")
            {
                message += "divers ";
            }
            if(giftId[i] == "990004")
            {
                message +="non martiale ";
            }
            if(giftId[i] == "990005")
            {
                message +="non magique " ;
            }
            if(giftId[i] == "990006")
            {
                message +="non divers ";
            }
            if(giftId[i] == "990007")
            {
                message +="au choix ";
            }
            message += "au niveau " + QString::number(_race->_gift.value(giftId[i])) +" offerte grâce à votre race (";
            message += _race->getName() + ").";
            QMessageBox::information(this, "Compétence offerte non prise", message);

        }
        output += tmp;
    }
    return output;
}

void CCharacter::updatePointsTotal(bool verbose)
{
    //Starting with race + carac
    qint32 value = _advantage_points;
    value += getReligionPoints();
    value += _race->getAttribute("cost");
    //Adjust health because it's 7.5/hp (fuck you andreas)
    if( (getAttribute("health") - _race->getAttribute("health")) % 2 == 0)
    {
        value += _d->getCaracCost()[0] * (getAttribute("health") - _race->getAttribute("health"))/2;
    }
    else
    {
        value += _d->getCaracCost()[0] * (getAttribute("health") + 1 - _race->getAttribute("health"))/2;
    }
    value += _d->getCaracCost()[1] * (getAttribute("strength") - _race->getAttribute("strength"));
    value += _d->getCaracCost()[2] * (getAttribute("agility") - _race->getAttribute("agility"));
    value += _d->getCaracCost()[3] * (getAttribute("reflexe") - _race->getAttribute("reflexe"));
    value += _d->getCaracCost()[4] * (getAttribute("willpower") - _race->getAttribute("willpower"));
    if( (getAttribute("mana") - _race->getAttribute("mana")) % 3 == 0)
    {
        value += _d->getCaracCost()[5] * (getAttribute("mana") - _race->getAttribute("mana"))/3;
    }
    else
    {
        value += _d->getCaracCost()[5] * (3 - (getAttribute("mana") - _race->getAttribute("mana"))%3
                                            + getAttribute("mana") - _race->getAttribute("mana"))/3;
    }
    QMap<QString, qint32> specialCost = _d->getSpecialCost();

    if(_race->getId() == "010005" || _race->getId() == "010026")//Demi elfe & pyrde
    {
        specialCost["t"] = 60;
        specialCost["g"] = 160;
    }
    qint32 skillCost = 0;
    for(int i=0;i<_skills.size();i++)
    {
        updateRaceSkillSpeed(i);
        skillCost += _d->getSkillCost(_skills[i]->getParam("speed"), _skills[i]->getParam("level").toInt());
        skillCost += specialCost.value(_skills[i]->getParam("special"));
    }

    value += skillCost;
    value -= raceGiftPoints(verbose);
    //TODO : gift
    _points = value;

}

void CCharacter::updateRaceSkillSpeed(qint32 skillIndex)
{
    if((_race->getId() == "010011" || _race->getId() == "010012")
            && _skills[skillIndex]->getParam("id") == "020048") //Technologie middle for nain/gnome
    {
        _skills[skillIndex]->setParam("speed", "m");

    }
    else if(_race->getId() == "010015" && _skills[skillIndex]->getParam("id") == "020047") //Portail middle for vaerilien
    {
        _skills[skillIndex]->setParam("speed", "m");
    }
    else if(_race->getId() == "010016" && _skills[skillIndex]->getParam("id") == "020024") // Psychique middle for troll ancien
    {
        _skills[skillIndex]->setParam("speed", "m");
    }
    //It's just here so if we have psychique (for ex) at middle, then switch to another race, it sets it back to slow
    else if(_skills[skillIndex]->getParam("id") == "020048" || _skills[skillIndex]->getParam("id") == "020047"
            || _skills[skillIndex]->getParam("id") == "020024")
    {
        _skills[skillIndex]->setParam("speed", "s");

    }
}

void CCharacter::setName(QString name)
{
    _name = name;
}

qint32 CCharacter::getPointsTotal()
{
    return _points;
}

QString CCharacter::getName()
{
    return _name;
}

QString CCharacter::getRaceName()
{
    return _race->getName();
}

QList<CUSkill *> CCharacter::getSkill()
{
    return _skills;
}

qint32 CCharacter::getPerkNumberForSkill(qint32 skillIndex)
{
    return _skills[skillIndex]->getPerkNumber();
}

/*
 * skillIndex is the id in _skills, not the skill Id
 * */
void CCharacter::setSkillParam(qint32 skillIndex, QString name, QString value)
{
    _skills[skillIndex]->setParam(name, value);
}

void CCharacter::updatePerkNumber(qint32 skillIndex, qint32 newNumber)
{
    qint32 oldNumber = _skills[skillIndex]->getPerkB().size();
   if(oldNumber < newNumber)
    {
        for(int i=oldNumber;i<newNumber;i++)
        {
            //We add the 1st perk of the skill (Ascendant for Arbalète for example)
            CPerk *perk;
            if(!_skills[skillIndex]->isMisc())
            {
                perk =_d->getDefaultPerkForSkill(_skills[skillIndex]->getParam("id"), 0);
            }
            else//We have a misc skill and fck them
            {
                qint32 level=0;
                if(_skills[skillIndex]->getParam("id")=="020048")//SUCE MES BOULES
                {
                    if(i!=0)
                    {
                        level = 2*i-1;
                    }
                }
                else
                {
                    level = i*2+1;
                }
                perk= _d->getDefaultPerkForSkill(_skills[skillIndex]->getParam("id"), level);
            }
            _skills[skillIndex]->addPerk(perk);
        }
    }
    else if (oldNumber > newNumber)
    {
        for(int i=oldNumber;i>newNumber;i--)
        {
            _skills[skillIndex]->removeLastPerk();
        }
    }
    else//Not very usefull, for clarity
    {
        return;
    }
}
