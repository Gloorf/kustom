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
#include "odt_reader.h"
#include "../kustom-sheet-creator/data.h"
COdtReader::COdtReader(CData *data)
{
    
    _d=data;
    input = new QFile("content.xml");
     If we can't open it, let's show an error message.
    if (!input->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening it";
        return;
    }
     Removes any device() or data from the reader
     * and resets its internal state to the initial state.

}

QString COdtReader::readNextCell(QXmlStreamReader& xml)
{
    
    while(xml.name()=="p" || xml.name()=="line-break" || xml.name()=="covered-table-cell")//We go at the next table-cell
    {
        xml.readNextStartElement();
    }

    xml.readNextStartElement();//We are at it, reading the next attribute (a <text:p>)
    return xml.readElementText(QXmlStreamReader::IncludeChildElements);

}

void COdtReader::parseFile()
{
    
    // QXmlStreamReader takes any QIODevice.
    QXmlStreamReader xml(input);
    QStringList attrName;
    attrName << "Titre_5f_comp";
    qint32 type=9;
    // We'll parse the XML until we reach end of it.
    while(!xml.atEnd() && !xml.hasError())
    {//"Mega_5f_titre"
       if(xml.readNextStartElement())
       {
           QXmlStreamAttributes attributes = xml.attributes();
           //We build the list of style libreoffice uses for our title
           if(attributes.value("style:parent-style-name").toString()=="Titre_5f_comp")
           {
               attrName << attributes.value("style:name").toString();
           }
           if(attributes.value("text:style-name").toString()=="Mega_5f_titre")
           {
               type++;//When we have the first title it goes to 10, then 11, then 12.
           }
           //Looking for the comp title
           for(int i=0;i<attrName.size();i++)
           {
               if(attributes.value("text:style-name").toString()==attrName[i])
               {
                   QString name=xml.readElementText(QXmlStreamReader::IncludeChildElements);
                   name.remove("\n");
                   name.remove(":");
                   name=name.simplified();
                   xml.readNext();
                   QString speedS=xml.readElementText(QXmlStreamReader::IncludeChildElements);
                   speedS.resize(1);
                   qint32 speed= (speedS=="R") ? 0 : 1 ;
                   speed = (speed==1 && speedS=="L") ? 2 : speed;
                   CSkill tmpS;
                   skillData tmpD;
                   tmpD.name=name;
                   tmpD.speed=speed;
                   tmpD.type=type;
                   tmpD.special=20;
                   tmpD.level=0;
                   tmpS.setInfo(tmpD);
                   skillB.append(tmpS);
               }
           }
           //Looking for the table itself
           for(int i=0;i<skillB.size();i++)
           {
               bool multiNameSkill=false;
               if((attributes.value("table:name").toString().indexOf("Investigation")!=-1 && skillB[i].getInfo().name=="Investigation / Chasse")
               || (attributes.value("table:name").toString().indexOf("Assemblage")!=-1 && skillB[i].getInfo().name=="Assemblage / Désassemblage")
               || (attributes.value("table:name").toString().indexOf("Discrétion")!=-1 && skillB[i].getInfo().name=="Discrétion / Escamotage"))
               {
                   multiNameSkill=true;
               }
               if(attributes.value("table:name").toString().replace("_", " ").toLower()==skillB[i].getInfo().name.toLower() || multiNameSkill)
               {//We have found a table for a skill, now we need to fill dat biatch !
                   //qDebug() << "Nom du tableau " << attributes.value("table:name") << "Nom du skill" << skillB[i].getInfo().name;
                   bool stillInTable=true;
                   bool row=false;
                   qint32 rowCount=0;
                   qint32 columnCount=0;//Used for misc skills
                   QStringList schoolList;
                   //<table:table-column table:style-name="Équitation.A" table:number-columns-repeated="3"/>

                   while(stillInTable)
                   {
                        if(xml.name()=="table-row")
                        {
                            row= !row;//If we begin a line, row is at false -> put it true ; if we end a line, it's at true -> false
                            if(xml.isStartElement())
                            {
                               rowCount++;
                            }

                        }
                        if(xml.name()=="table-cell" && rowCount==1 && xml.isStartElement())
                        {
                            columnCount++;
                        }
                        if(row && rowCount>1)//Skip the 1st row
                        {

                            if(skillB[i].getInfo().name=="Défense" || skillB[i].getInfo().name=="Lutte" || skillB[i].getInfo().type==10)//Weapon specific treatment
                            {
                                //we are at table-row ; we need to skip one (go to table-cell), then one other (go to text:p)
                                perkData tmpStruct;
                                xml.readNext();
                                xml.readNext();
                                tmpStruct.name=xml.readElementText(QXmlStreamReader::IncludeChildElements);
                                //Got name
                                xml.readNextStartElement();
                                tmpStruct.cost=xml.readElementText(QXmlStreamReader::IncludeChildElements).left(1).toInt();
                                QString tmpText, tmpNumber;
                                tmpText=readNextCell(xml);
                                for(int k=0;k<tmpText.size();k++)
                                {
                                    if(tmpNumber.size()>0 && !tmpText[k].isNumber())
                                    {    //If we already have at least 1 digit and no digit after -> the additionnal condition we are looking for
                                        tmpStruct.levelNeeded=tmpText.remove(0,k);
                                        break;
                                    }
                                    if(tmpText[k].isNumber())
                                    {
                                        tmpNumber.append(tmpText[k]);
                                    }
                                }
                                tmpStruct.level=tmpNumber.toInt();

                                tmpStruct.desc=readNextCell(xml);

                                tmpStruct.cooldown=readNextCell(xml);

                                tmpStruct.misc=readNextCell(xml);
                                skillB[i].addPerk(tmpStruct);
                            }
                            else if(skillB[i].getInfo().type==11) // Magic specific treatment
                            {
                                //we are at table-row ; we need to skip one (go to table-cell), then one other (go to text:p)
                                perkData tmpStruct;
                                xml.readNext();
                                xml.readNext();
                                tmpStruct.name=xml.readElementText(QXmlStreamReader::IncludeChildElements);
                                xml.readNextStartElement();
                                tmpStruct.cost=xml.readElementText(QXmlStreamReader::IncludeChildElements).left(1).toInt();
                                //For some reason it failed (libreoffice problem with <line-break:/> ...), adjust the cost
                                if(tmpStruct.name.indexOf("pts")!=-1)
                                {
                                    tmpStruct.cost=tmpStruct.name.left(tmpStruct.name.indexOf("pts")).right(2).toInt();
                                    tmpStruct.name=tmpStruct.name.left(tmpStruct.name.indexOf("pts"));
                                    tmpStruct.name.resize((tmpStruct.name.size()-3));
                                }
                                tmpStruct.level=0;//No level needed to buy a skill with magic !
                                if(skillB[i].getInfo().name=="Psychique" || skillB[i].getInfo().name=="Élémentaire"
                                || skillB[i].getInfo().name=="Moniale"   || skillB[i].getInfo().name=="Occultisme")
                                {
                                    tmpStruct.school=readNextCell(xml);
                                    if(!schoolList.contains(tmpStruct.school))
                                    {
                                        schoolList << tmpStruct.school;
                                    }
                                }
                                tmpStruct.dd=readNextCell(xml);
                                if(skillB[i].getInfo().name=="Sceaux")
                                {
                                    tmpStruct.incantTime=readNextCell(xml);
                                }
                                if(skillB[i].getInfo().name=="Sorcellerie")
                                {
                                    tmpStruct.optionalComponent=readNextCell(xml);
                                }
                                if(skillB[i].getInfo().name=="Moniale")
                                {
                                    tmpStruct.condition=readNextCell(xml);
                                }
                                tmpStruct.desc=readNextCell(xml);
                                if(skillB[i].getInfo().name!="Sceaux")//Sceaux don't have CD
                                {
                                    tmpStruct.cooldown=readNextCell(xml);
                                }
                                tmpStruct.misc=readNextCell(xml);
                                if(tmpStruct.misc.indexOf("Incantation lente")!= -1)
                                {
                                    tmpStruct.slowIncant=true;
                                    tmpStruct.misc.replace("Incantation lente.", "");
                                }
                                if(tmpStruct.misc.indexOf("Incantation instantanée")!= -1)
                                {
                                    tmpStruct.fastIncant=true;
                                    tmpStruct.misc.replace("Incantation instantanée.", "");
                                }
                                if(tmpStruct.misc.indexOf("Jet de concentration requis à chaque tour")!=-1)
                                {
                                    tmpStruct.focus=true;
                                    tmpStruct.misc.replace("Jet de concentration requis à chaque tour.", "");
                                }
                                skillB[i].addPerk(tmpStruct);
                            }
                            else if(skillB[i].getInfo().type==12 && skillB[i].getInfo().name!="Forge technomagique" //Misc treatment
                                    && skillB[i].getInfo().name!="Défense" && skillB[i].getInfo().name!="Lutte")
                            {
                                xml.readNext();
                                xml.readNext();
                                QString tmpText=xml.readElementText(QXmlStreamReader::IncludeChildElements);
                                qint32 level=tmpText.remove(0,tmpText.lastIndexOf(" ")).toInt();
                                for(int j=0;j<columnCount-1;j++)//Don't use the 1st column (level only)
                                {
                                    perkData tmpStruct;
                                    QString tmpText2;
                                    tmpText2=readNextCell(xml);

                                    tmpStruct.name=tmpText2.left(tmpText2.indexOf(":")).simplified();
                                    tmpStruct.level=level;
                                    tmpStruct.cost=0;
                                    tmpText2.remove(0, tmpText2.indexOf(":")+1);
                                    tmpStruct.desc=tmpText2.simplified();
                                    skillB[i].addPerk(tmpStruct);
                                }
                            }

                        }
                        if(xml.name()=="table" && xml.isEndElement())
                        {
                            stillInTable=false;
                        }
                        xml.readNext();
                   }//While : stillInTable

                   skillData tmpD=skillB[i].getInfo();
                   tmpD.school=schoolList;
                   skillB[i].setInfo(tmpD);

               }//if : table found
           }//For : looking for table
       }//While  xmlReadNextStartElement
    }//
    xml.clear();
}

void COdtReader::saveFile()
{

    QFile file( "out/skill.xml" );
     if( !file.open( QIODevice::WriteOnly ) )
     {
         qDebug() << "argh je peux pas écrire";
     }
     QXmlStreamWriter stream(&file);
     stream.setAutoFormatting(true);
     stream.writeStartDocument();
     stream.writeStartElement("skills");
     qint32 realPerkInd=0;
     for(int i=0;i<skillB.size();i++)
     {
         skillData info = skillB[i].getInfo();
         stream.writeStartElement("skill");
         stream.writeTextElement("id", _d->getFullId("skill",i));
         stream.writeTextElement("name", info.name);
         QString speed= info.speed == 2 ? "s" : (info.speed == 1 ? "m" : "f");
         QString type= info.type == 12 ? "d" : (info.type==11 ? "m" : "w");
         stream.writeTextElement("speed", speed);
         stream.writeTextElement("type", type);
         stream.writeStartElement("perks");
         for(int k=0;k<skillB[i].getPerkB().size();k++)
         {
            perkData perk = skillB[i].getPerkB()[k];
            stream.writeStartElement("perk");
            stream.writeTextElement("id",  _d->getFullId("perk", realPerkInd));
            stream.writeTextElement("name", perk.name);
            stream.writeTextElement("cost", QString::number(perk.cost));
            stream.writeTextElement("level", QString::number(perk.level));
            stream.writeTextElement("desc", perk.desc);
            stream.writeTextElement("misc", perk.misc);
            stream.writeTextElement("cooldown",perk.cooldown);
            stream.writeTextElement("dd", perk.dd);
            QString slow = perk.slowIncant ? "true" : "false";
            stream.writeTextElement("slowIncant", slow);
            QString fast = perk.fastIncant ? "true" : "false";
            stream.writeTextElement("fastIncant", fast);
            QString focus = perk.focus ? "true" : "false";
            stream.writeTextElement("focus", focus);
            stream.writeTextElement("condition", perk.condition);
            stream.writeTextElement("school", perk.school);
            stream.writeTextElement("optionalComponent", perk.optionalComponent);
            stream.writeTextElement("incantTime", perk.incantTime);

            //ADD : prerequisite
            stream.writeStartElement("prerequisite");
            //"Maîtrise du combat rapproché"
            //"Perception ou Survie 1"
            QString needed = perk.levelNeeded;
            if(perk.levelNeeded=="")
            {
                stream.writeAttribute("type", "none");
            }
            else if(perk.levelNeeded.indexOf("ou")!=-1 &&
                    perk.levelNeeded.at(perk.levelNeeded.length()-1).isDigit())
            {
                stream.writeAttribute("type", "skill");
                stream.writeAttribute("number", "2");
                needed = perk.levelNeeded.left(perk.levelNeeded.length()-2);
                QString level = perk.levelNeeded.at(perk.levelNeeded.length()-1);
                //If we have a double-digit level prerequisite (? doesn't exist i think but ...)

                if(needed.at(needed.length()-1).isDigit())
                {
                    needed = needed.left(needed.length()-1);
                }
                QString need1, need2, id1, id2;
                need1 = needed.left(needed.indexOf("ou")).simplified();
                need2=needed.remove(0,needed.indexOf("ou")+2).simplified();//+2 for the "ou"
                if(_d->findSkillIndex(need1)!=-1)
                {
                    id1=_d->getFullId("skill", _d->findSkillIndex(need1));
                }
                else
                {
                    id1=_d->getFullId("misc", 0);
                }
                if(_d->findSkillIndex(need2)!=-1)
                {
                    id2=_d->getFullId("skill", _d->findSkillIndex(need2));
                }
                else
                {
                    id2=_d->getFullId("misc", 0);
                }
                stream.writeTextElement("id1", id1);
                stream.writeTextElement("id2", id2);
                stream.writeTextElement("level", level);
            }
            else if(perk.levelNeeded.at(perk.levelNeeded.length()-1).isDigit())
            {
                stream.writeAttribute("type", "skill");
                needed = perk.levelNeeded.left(perk.levelNeeded.length()-2);
                QString level = perk.levelNeeded.at(perk.levelNeeded.length()-1);
                //If we have a double-digit level prerequisite (? doesn't exist i think but ...)

                if(needed.at(needed.length()-1).isDigit())
                {
                    needed = needed.left(needed.length()-1);
                }
                QString id="";
                if(_d->findSkillIndex(needed) != -1)
                {
                    id=_d->getFullId("skill", _d->findSkillIndex(needed));
                }
                else if(needed=="Arme")//Fcking special case
                {
                    id=_d->getFullId("misc", 1);
                }
                else
                {
                    id=_d->getFullId("misc", 0);
                }
                stream.writeTextElement("id", id);
                stream.writeTextElement("level", level);
            }
            else
            {
                needed = perk.levelNeeded;
                stream.writeAttribute("type", "perk");
                //We need to do another for because we need to check every perk u_u
                QString id="";
                for(int l=0;l<skillB[i].getPerkB().size();l++)
                {
                    if(skillB[i].getPerkB()[l].name==needed)
                    {
                        //just offset realPerkInd to get the good index
                        id=_d->getFullId("perk", realPerkInd+(l-k));
                        break;
                    }

                }
                id = id.isEmpty() ? _d->getFullId("misc", 0) : id;
                stream.writeTextElement("id", id);

            }
            stream.writeEndElement();//prerequisite
            stream.writeEndElement();//perk
            realPerkInd++;
         }
         stream.writeEndElement();//"perkS"
         stream.writeEndElement();//"skill"
     }
     stream.writeEndElement();//"skillS"
     stream.writeEndDocument();
     file.close();

}
*/
