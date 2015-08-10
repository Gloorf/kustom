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
#include <QtWidgets>
#include <QDebug>
#include "data_type.h"

CPerk::CPerk()
{

}

qint32 CPerk::getParamInt(QString name)
{
    return _info.value(name).toInt();
}
QString CPerk::getParamStr(QString name)
{
    return _info.value(name);
}

bool CPerk::getParamBool(QString name)
{
    return _info.value(name) == "true";
}

void CPerk::setParam(QString name, QString value)
{
    _info.insert(name, value);
}

void CPerk::setParam(QString name, qint32 value)
{
    _info.insert(name, QString::number(value));
}

void CPerk::setPrerequisite(QString id, qint32 level)
{
    _prerequisite.insertMulti(id, level);
}

bool CPerk::havePrerequisite()
{
    return !_prerequisite.keys().size()==0;
}

bool CPerk::IsPrerequisitePerk()
{
    if (havePrerequisite() && _prerequisite.keys()[0].left(2) == "03")//It is a perk
    {
        return true;
    }
    return false;
}

QMap<QString, qint32> CPerk::getPrerequisite()
{
    return _prerequisite;
}

CSkill::CSkill()
{
    _info["id"]="000000";
    _info["name"]="";
    _info["speed"]="";
    _info["type"]="";
    _info["special"]="";
    _info["level"]="";
}

QString CSkill::getParam(QString name)
{
    if(name=="schools" && _info.count(name)>1)
    {
        return QStringList(_info.values(name)).join(",");
    }
    else
    {
        return _info.value(name);
    }
}
void CSkill::setParam(QString name, QString value)
{
    if(name=="schools")
    {
        _info.insertMulti(name, value);
    }
    else
    {
        _info[name]=value;
    }
}

void CSkill::addPerk(CPerk *perk)
{
    _perkB.append(perk);
}
QList<CPerk *> CSkill::getPerkB()
{
    return _perkB;
}



CPerk * CSkill::findPerk(QString name)
{
    for(int i=0;i<_perkB.size();i++)
    {
        if(_perkB[i]->getParamStr("name")==name)
        {
            return _perkB[i];
        }
    }
    CPerk *output = new CPerk();
    return output;
}

void CSkill::removeAllPerk()
{
    qint32 tmpSize=_perkB.size();
    for(int i=0;i<tmpSize;i++)
    {
        _perkB.removeLast();
    }
}
/*
 * Weapon +  défense + lutte
 * */
bool CSkill::isWeapon()
{
    if(_info.value("type")=="w" || _info.value("name")=="Défense" || _info.value("name")=="Lutte")
    {
        return true;
    }
    return false;
}
/*
 * Moniale + psychique + élémentaire + occultisme
 * */
bool CSkill::multipleSchool()
{
    if(_info.value("name")=="Moniale" || _info.value("name")=="Psychique" || _info.value("name")=="Élémentaire" || _info.value("name")=="Occultisme")
    {
        return true;
    }
    return false;
}

bool CSkill::isMisc()
{
    if(_info.value("type")=="d" && _info.value("name")!="Défense" && _info.value("name")!="Lutte")
    {
        return true;
    }
    return false;
}

bool CSkill::isMagic()
{
    if(_info.value("type")=="m")
    {
        return true;
    }
    return false;
}

qint32 CSkill::columnCount()
{
    qint32 output=0;
    if(isMisc())
    {
        output++;//First column (Pallier de compétence + Comptence 1/3/5/7/10 etc)
        for(int i=0;i<_perkB.size();i++)
        {
            if(_perkB[i]->getParamInt("level")==1)//We count how many perk we have at one level
            {
                output++;
            }
        }
    }
    else if (isMagic())
    {
        output=5;
        output = multipleSchool() ? output+1 : output;
        if(_info.value("name")=="Sorcellerie" || _info.value("name")=="Moniale")
        {
            output++;
        }
    }
    else if (isWeapon())
    {
        output=5;
    }
    return output;
}

qint32 CSkill::rowCount()
{
    qint32 output=0;
    if(isMisc())
    {
        output=output + _perkB.size()/(columnCount()-1);//columnCount()-1 cause of header
    }
    else if(isMagic())
    {
        output=output + _perkB.size();
    }
    else if(isWeapon())
    {
        for(int i=0;i<_perkB.size();i++)
        {
            if(_perkB[i]->getParamInt("level")<=_info.value("level").toInt())
            {
                output++;
            }
        }
    }
    return output;

}

QStringList CSkill::headerLabels()
{
    QStringList output;
    if(isWeapon())
    {
        output << "Technique" << "Pré-requis" << "Effet" << "α" << "Divers";
    }
    else if(isMagic())
    {

        if(_info.value("name")=="Moniale")
        {
            output << "Sort" << "École" << "DD" << "Condition" << "Effet" << "α" << "Divers";
        }
        if(multipleSchool())
        {
            output << "Sort" << "École" << "DD" << "Effet" << "α" << "Divers";
        }
        else if(_info.value("name")=="Sceaux")
        {
            output << "Sceau" << "DD" << "Temps" << "Effet" << "Divers";
        }
        else if(_info.value("name")=="Sorcellerie")
        {
            output << "Sort" << "DD" << "Composantes" << "Effet" << "α" << "Divers";
        }
        else
        {
            output << "Sort" << "DD" << "Effet" << "α" << "Divers";
        }
    }
    else if (isMisc())
    {
        output << "Pallier de compétences";
        for(int i=0;i<columnCount()-1;i++)
        {
            output << "Aptitude";
        }
    }
    return output;
}

QStringList CSkill::getSchools()
{
    QStringList output;
    for(int i=0;i<_perkB.size();i++)
    {
        if(!output.contains(_perkB[i]->getParamStr("school")))
        {
            output << _perkB[i]->getParamStr("school");
        }
    }
    return output;
}

CUSkill::CUSkill(CSkill *base)
{
    setParam("id",base->getParam("id"));
    setParam("name", base->getParam("name"));
    setParam("speed", base->getParam("speed"));
    setParam("type",base->getParam("type"));
}

qint32 CUSkill::getPerkNumber()
{
    qint32 output=0;
    if(!isMisc())
    {
        output = getParam("level").toInt();
        if(getParam("special")=="g")
        {
            output +=1;
        }
    }
    else
    {
        output = (getParam("level").toInt() +1)/2;
        output = getParam("level").toInt() == 9 ? output - 1 : output;//3 level between 9 & 10
        if(getParam("id")=="020048")//Did i told you the story of motherfucking technologie ?
        {
            output +=1;
        }
    }
    return output;
}

void CUSkill::removeLastPerk()
{
    QList<CPerk *> perks = getPerkB();
    removeAllPerk();
    for(int i=0;i<perks.size()-1;i++)
    {
        addPerk(perks[i]);
    }
}

void CUSkill::updatePerkId(qint32 perkIndex, CPerk *newPerk)
{
    _perkB[perkIndex]=newPerk;
}
/*
 * Check if we have too many perks for our level, related to their cost
 * */

qint32 CUSkill::excessPerkNumber()
{
    qint32 numberAllowed = getPerkNumber();
    qint32 realNumber=0;
    for(int i=0;i<_perkB.size();i++)
    {
        realNumber += _perkB[i]->getParamInt("cost");
    }
    if(realNumber <= numberAllowed)
    {
        return 0;
    }
    else
    {
        return realNumber - numberAllowed;
    }
}

void CUSkill::setSchool(QString school)
{
    _chosenSchool = school;
}

QString CUSkill::getSchool()
{
    return _chosenSchool;
}

CRace::CRace(QString id)
{
    _id=id;
    _name="";
    _carac["health"]=0;
    _carac["strength"]=0;
    _carac["agility"]=0;
    _carac["reflexe"]=0;
    _carac["willpower"]=0;
    _carac["mana"]=0;
    _carac["cost"]=0;
}

void CRace::setParam(QString name, QString value)
{
    if(name=="id")
    {
        _id=value;
    }
    else if(name=="name")
    {
        _name=value;
    }
    else
    {
        qDebug() << "Error : trying to set unknown param " << name << "with value" << value;
    }
}

void CRace::setParam(QString name, qint32 value)
{
    _carac[name]=value;
}

void CRace::addGift(QString id, qint32 level)
{
    _gift.insertMulti(id, level);
}

qint32 CRace::getAttribute(QString name)
{
    return _carac.value(name);
}
QString CRace::getName()
{
    return _name;
}

QString CRace::getId()
{
    return _id;
}

CReligion::CReligion()
{
    _id="040000";
    _name="";
}

void CReligion::addCompatibility(QString id, QString state)
{
    _compatibilities[id]=state;
}

void CReligion::addPower(religionPower power)
{
    _powers << power;
}

void CReligion::setParam(QString name, QString value)
{
    if(name=="id")
    {
        _id=value;
    }
    else if(name=="name")
    {
        _name=value;
    }
    else
    {
        qDebug() << "Error : trying to set unknown param " << name << "with value" << value;
    }
}

QString CReligion::getId()
{
    return _id;
}

QString CReligion::getName()
{
    return _name;
}

religionPower CReligion::getPowerById(QString id)
{
    for(int i=0;i<_powers.size();i++)
    {
        if(_powers[i].id==id)
        {
            return _powers[i];
        }
    }
    religionPower output;
    output.id="-1";
    return output;
}

QList<religionPower> CReligion::getPowerList()
{
    QList<religionPower> output;
    for(int i=0;i<_powers.size();i++)
    {
        output << _powers[i];
    }
    return output;
}

QMap<QString, QString> CReligion::getCompatibilities()
{
    return _compatibilities;
}

religionPower CReligion::getCult()
{
    return _powers[0];
}

void CReligion::removePower(QString id)
{
    for (int i=0;i<_powers.size();i++)
    {
        if(_powers[i].id==id)
        {
            _powers.removeAt(i);
        }
    }
}

void CReligion::removeAllPower()
{
    qint32 size = _powers.size();
    for(int i=0;i<size;i++)
    {
        _powers.removeLast();
    }
}

CVerticalScrollArea::CVerticalScrollArea(QWidget *widget)
{
    setWidget(widget);
    setWidgetResizable(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

bool CVerticalScrollArea::eventFilter(QObject *o, QEvent *e)
{
    // This works because QScrollArea::setWidget installs an eventFilter on the widget
    if(o && o == widget() && e->type() == QEvent::Resize)
    setMinimumWidth(widget()->minimumSizeHint().width() + verticalScrollBar()->width());
    return QScrollArea::eventFilter(o, e);
}
