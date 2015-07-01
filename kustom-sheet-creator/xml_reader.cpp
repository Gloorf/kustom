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

#include <QApplication>
#include <QXmlStreamReader>
#include <QtCore>
#include <QtGui>
#include <QDebug>

#include <QtWidgets>
#include "xml_reader.h"

CXmlReader::CXmlReader()
{
}

QList<QList<QList<damageData> > > CXmlReader::loadDamage()
{
    QFile file( "data/damage.xml");
    QList<QList<QList<damageData> > > damageB;
    if( !file.open( QIODevice::ReadOnly | QIODevice::Text) )
    {
        qDebug() <<  "Impossible de charger data/damage.xml";
    }
    QXmlStreamReader xml(&file);
    while(!xml.atEnd() && !xml.hasError())
    {
        if(xml.readNextStartElement())
        {
            xml.readNextStartElement();
            while(xml.name()!="damages" && !xml.isEndElement())
            {
                if(xml.name()=="bodyPart")
                {
                    bodyPartName << xml.attributes().value("name").toString();
                }
                else
                {
                }
                QStringList tmpZoneName;
                xml.readNextStartElement();
                QList<QList<damageData> >  partDmg;

                while(xml.name()!="bodyPart" && !xml.isEndElement())
                {
                    if(xml.name()=="bodyZone")
                    {
                        tmpZoneName << xml.attributes().value("name").toString();
                    }
                    xml.readNextStartElement();

                    QList<damageData> zoneDmg;
                    while(xml.name()!="bodyZone" && !xml.isEndElement())
                    {
                        while(xml.name()=="damage" && !xml.isEndElement())
                        {
                            xml.readNextStartElement();
                            damageData rollDmg;
                            rollDmg.value=xml.readElementText().toInt();
                            xml.readNextStartElement();
                            rollDmg.roll=xml.readElementText().toInt();
                            xml.readNextStartElement();
                            rollDmg.bonus=xml.readElementText().toInt();
                            xml.readNextStartElement();
                            rollDmg.max=xml.readElementText() == "true" ? true : false;
                            xml.readNextStartElement();
                            xml.readNextStartElement();
                            zoneDmg << rollDmg;
                        }
                        partDmg << zoneDmg;
                        xml.readNextStartElement();
                    }
                }
                bodyZoneName << tmpZoneName;
                damageB << partDmg;

                xml.readNextStartElement();
            }
            xml.readNextStartElement();
        }
        xml.readNextStartElement();
    }

    file.close();
    return damageB;
}

QList<CRace *> CXmlReader::loadRace()
{
    QFile file( "data/race.xml" );
    QList<CRace *> raceB;
     if( !file.open( QIODevice::ReadOnly | QIODevice::Text) )
     {
         qDebug() << "Impossible de charger data/race.xml";
     }
    QXmlStreamReader xml(&file);
    while(!xml.atEnd() && !xml.hasError())
    {
        if(xml.readNextStartElement())
        {
            QXmlStreamAttributes attributes = xml.attributes();
            if(xml.name()=="races")
            {
                xml.readNextStartElement();//go to race

                while(xml.name()!="races" && !xml.isEndElement())
                {

                    while(xml.name()=="race" && !xml.isEndElement())
                    {
                        CRace *tmpRace = new CRace();

                        xml.readNextStartElement();
                        while(xml.name()!="race")
                        {
                            QString value=xml.readElementText();
                            bool ok;
                            value.toInt(&ok);
                            if(ok && xml.name() != "id")//id can be int too but it fucks upeverything
                            {
                                tmpRace->setParam(xml.name().toString(), value.toInt());
                             }
                            else
                            {
                                tmpRace->setParam(xml.name().toString(), value);
                            }
                            xml.readNextStartElement();
                            if(xml.name()=="gift")
                            {
                                if(xml.attributes().value("number").toString().toInt()==2)
                                {
                                    xml.readNextStartElement();//go to id1

                                    QString id1=xml.readElementText();
                                    xml.readNextStartElement();//id2

                                    QString id2=xml.readElementText();
                                    xml.readNextStartElement();//go to lvl

                                    qint32 level= xml.readElementText().toInt();
                                    tmpRace->addGift(id1,level);
                                    tmpRace->addGift(id2,level);
                                    xml.readNextStartElement();//move after lvl

                                    xml.readNextStartElement();
                                }
                                else
                                {

                                    xml.readNextStartElement();//go to id
                                    QString id=xml.readElementText();
                                    xml.readNextStartElement();//go to lvl

                                    qint32 level = xml.readElementText().toInt();
                                    tmpRace->addGift(id,level);

                                    xml.readNextStartElement();//move after lvl

                                    xml.readNextStartElement();
                                    //qDebug() << xml.name() << xml.isEndElement();
                                }
                            }
                        }
                        xml.readNextStartElement();
                        raceB << tmpRace;
                    }//Race parsing finished, save it + go to next
                }
            }
        }
    }
    file.close();
    return raceB;
}

QList<CReligion *> CXmlReader::loadReligion()
{
    QFile file( "data/religion.xml" );
    QList<CReligion *> religionB;
     if( !file.open( QIODevice::ReadOnly | QIODevice::Text) )
     {
         qDebug() << "Impossible de charger data/religion.xml";
     }
    QXmlStreamReader xml(&file);
    while(!xml.atEnd() && !xml.hasError())
    {
        if(xml.readNextStartElement())
        {
            QXmlStreamAttributes attributes = xml.attributes();
            if(xml.name()=="religions")
            {
                xml.readNextStartElement();//go to religion
                while(xml.name()!="religions" && !xml.isEndElement())
                {
                    CReligion *tmpReligion = new CReligion();

                    //xml.readNextStartElement();//go to religion?

                    while(xml.name()=="religion" && !xml.isEndElement())
                    {
                        xml.readNextStartElement();
                        QString id = xml.readElementText();
                        tmpReligion->setParam("id", id);
                        xml.readNextStartElement();//go to name

                        QString name = xml.readElementText();
                        tmpReligion->setParam("name",name);
                        xml.readNextStartElement();//go to <religionPower>

                        while(xml.name()=="religionPower" && !xml.isEndElement())
                        {
                            religionPower power;
                            xml.readNextStartElement();
                            power.id=xml.readElementText();
                            xml.readNextStartElement();
                            power.name=xml.readElementText();
                            xml.readNextStartElement();
                            power.cult=xml.readElementText()=="true" ? true : false;
                            xml.readNextStartElement();
                            power.cost=xml.readElementText().toInt();
                            xml.readNextStartElement();
                            power.special=xml.readElementText().toInt();
                            tmpReligion->addPower(power);
                            xml.readNextStartElement();
                            xml.readNextStartElement();//double jump OO
                        }

                        if(xml.name() == "compatibilities" && !xml.isEndElement())
                        {
                            xml.readNextStartElement();//jump to compatibilitY

                            while(xml.name() == "compatibility" && !xml.isEndElement())
                            {
                                xml.readNextStartElement();//jump to id
                                QString id=xml.readElementText();
                                xml.readNextStartElement();//jump to stace
                                QString state=xml.readElementText();
                                tmpReligion->addCompatibility(id, state);
                                xml.readNextStartElement();
                                xml.readNextStartElement();
                            }
                        }

                    }
                    religionB << tmpReligion;

                    xml.readNextStartElement();//double jump ???????????? trop chelou
                    xml.readNextStartElement();//jump from one religion to the other

                }


            }
        }
    }
    return religionB;
}

QList<CSkill *> CXmlReader::loadSkill()
{

    QFile file( "data/skill.xml");
    QList<CSkill *> skillB;
    if( !file.open( QIODevice::ReadOnly | QIODevice::Text) )
    {
        qDebug() << "Impossible de charger data/skill.xml";
    }
    QXmlStreamReader xml(&file);
    while((!xml.atEnd() && !xml.hasError()))
    {

        if(xml.readNextStartElement())
        {

            xml.readNextStartElement();
            while(xml.name() !="skills" && !xml.isEndElement())
            {

                while(xml.name() == "skill" && !xml.isEndElement())
                {
                    CSkill *tmpSkill = new CSkill();

                    xml.readNextStartElement();
                    tmpSkill->setParam("id", xml.readElementText());
                    xml.readNextStartElement();
                    tmpSkill->setParam("name", xml.readElementText());
                    xml.readNextStartElement();
                    tmpSkill->setParam("speed", xml.readElementText());
                    xml.readNextStartElement();
                    tmpSkill->setParam("type", xml.readElementText());
                    xml.readNextStartElement();//"<perks>"
                    xml.readNextStartElement();//"<perk>"


                    while(xml.name()=="perk" && !xml.isEndElement())
                    {
                        CPerk *perk = new CPerk();
                        xml.readNextStartElement();
                        while(xml.name()!="perk")
                        {
                            perk->setParam(xml.name().toString(), xml.readElementText());
                            xml.readNextStartElement();
                            if(xml.name()=="prerequisite")
                            {
                                if (xml.attributes().value("type")=="none")
                                {
                                    xml.readNextStartElement();
                                }
                                else if(xml.attributes().value("type")=="perk")
                                {
                                    xml.readNextStartElement();
                                    perk->setPrerequisite(xml.readElementText());
                                    xml.readNextStartElement();
                                }
                                else if(xml.attributes().value("type")=="skill")
                                {
                                    xml.readNextStartElement();
                                    QString id=xml.readElementText();
                                    QString id2="";
                                    bool multi=false;
                                    if(xml.name()=="id1")
                                    {
                                        xml.readNextStartElement();
                                        id2=xml.readElementText();
                                        multi=true;
                                    }
                                    xml.readNextStartElement();
                                    qint32 level = xml.readElementText().toInt();
                                    if(multi)
                                    {
                                        perk->setPrerequisite(id2, level);
                                    }
                                    perk->setPrerequisite(id, level);
                                    xml.readNextStartElement();
                                }
                                //xml.readNextStartElement();
                            }
                        }
                        xml.readNextStartElement();//Switch to next perk
                        tmpSkill->addPerk(perk);
                    }
                    xml.readNextStartElement();//Switch to
                    xml.readNextStartElement();
                    skillB << tmpSkill;
                }
            }
        }
    }
    return skillB;
}
