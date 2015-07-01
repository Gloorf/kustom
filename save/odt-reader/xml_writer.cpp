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
/*
#include <QApplication>
#include <QXmlStreamReader>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include "xml_writer.h"
#include "../kustom-sheet-creator/data.h"
/*
CXmlWriter::CXmlWriter(CData *d)
{
    _d=d;
}
/*
 * À virer + tard
 * 
damageData CData::dDM(qint32 value, qint32 roll, qint32 bonus, bool max)
{
    damageData tmpStruct;
    tmpStruct.value=value;
    tmpStruct.roll=roll;
    tmpStruct.bonus=bonus;
    tmpStruct.max=max;
    return tmpStruct;
}
void CXmlWriter::saveRace()
{
    QList<QList<qint32> > raceB;
    QList<qint32> humanBase, elfBase, forestElfBase, drowBase, bloodElfBase,
                  halfElfBase, highElfBase, whiteElfBase, furyBase, halflingBase,
                  orcBase, dwarfBase, gnomeBase, goblinBase, gluhgBase, vaerilienBase,
                  elderTrollBase, trollBase, dragonGBase, dragonTGBase, dragonCBase,
                  ignasBase, mirfBase, huneadBase, yetsBase, nyctereBase, pyrdeBase, inshumBase,
                  vaerilienGuardBase, shadowBase;
    humanBase << 39 << 13 << 6 << 8 << 4 << 15 << 0;
    elfBase << 29 << 8 << 9 << 7 << 6 << 30 << 30;
    forestElfBase << 24 << 8 << 13 << 8 << 4 << 8 << 25;
    drowBase << 37 << 13 << 8 << 9 << 7 << 25 << 90;
    bloodElfBase << 29 << 10 << 8 << 10 << 12 << 60 << 140;
    halfElfBase << 38 << 12 << 8 << 8 << 6 << 20 << 70;
    highElfBase << 29 << 12 << 9 << 9 << 10 << 35 << 90;
    whiteElfBase << 27 << 8 << 9 << 7 << 8 << 35 << 150;
    furyBase << 24 << 7 << 8 << 7 << 5 << 13 << 130;
    halflingBase << 25 << 6 << 13 << 9 << 4 << 5 << 0;
    orcBase << 44 << 16 << 5 << 7 << 4 << 3 << 50;
    dwarfBase << 48 << 13 << 3 << 7 << 4 << 3 << 30;
    gnomeBase << 39 << 8 << 5 << 5 << 5 << 14 << 20;
    goblinBase << 16 << 4 << 14 << 8 << 2 << 0 << -50;
    gluhgBase << 26 << 6 << 9 << 5 << 4 << 10 << 70;
    vaerilienBase << 37 << 12 << 6 << 8 << 6 << 20 << 170;
    elderTrollBase << 41 << 16 << 7 << 7 << 6 << 10 << 110;
    trollBase << 50 << 20 << 4 << 6 << 2 << 3 << 80;
    dragonGBase << 60 << 24 << 3 << 5 << 8 << 30 << 240;
    dragonTGBase << 65 << 26 << 2 << 5 << 8 << 32 << 240;
    dragonCBase << 72 << 30 << 1 << 5 << 8 << 34 << 240;
    ignasBase << 36 << 14 << 7 << 7 << 6 << 14 << 145;
    mirfBase << 28 << 10 << 4 << 6 << 3 << 3 << -20;
    huneadBase << 26 << 6 << 12 << 8 << 3 << 2 << 0;
    yetsBase << 39 << 13 << 8 << 8 << 6 << 20 << 100;
    nyctereBase << 22 << 6 << 7 << 6 << 3 << 10 << 0;
    pyrdeBase << 50 << 19 << 5 << 7 << 3 << 8 << 100;
    inshumBase << 43 << 18 << 6 << 9 << 4 << 4 << 80;
    vaerilienGuardBase << 60 << 20 << 6 << 6 << 0 << 60 << 50;
    shadowBase << 24 << 4 << 11 << 6 << 9 << 40 << 130;
    raceB << humanBase << elfBase << forestElfBase << drowBase << bloodElfBase << halfElfBase
             << highElfBase << whiteElfBase << furyBase << halflingBase << orcBase << dwarfBase
             << gnomeBase << goblinBase << gluhgBase << vaerilienBase << elderTrollBase
             << trollBase << dragonGBase << dragonTGBase << dragonCBase << ignasBase << mirfBase
             << huneadBase << yetsBase
             << nyctereBase << pyrdeBase << inshumBase << vaerilienGuardBase << shadowBase;
    QStringList raceName;
    raceName << QObject::tr("Humain") << QObject::tr("Elfe") << QObject::tr("Elfe Sylvain") << QObject::tr("Drow")
             << QObject::tr("Elfe de Sang") << QObject::tr("Demi Elfe") << QObject::tr("Haut Elfe")
             << QObject::tr("Elfe Blanc") << QObject::tr("Fury") << QObject::tr("Halfelin") << QObject::tr("Orc") << QObject::tr("Nain")
             << QObject::tr("Gnome") << QObject::tr("Gobelin") << QObject::tr("Gluhg")
             << QObject::tr ("Vaerilien") << QObject::tr("Troll ancien") << QObject::tr("Troll") << QObject::tr("Dragon G")
             << QObject::tr("Dragon TG") << QObject::tr("Dragon C") << QObject::tr("Ignas") << QObject::tr("Mirf") << QObject::tr("Hunead")
             << QObject::tr("Yets") << QObject::tr("Nyctère") << QObject::tr("Pyrde") << QObject::tr("Inshum") << QObject::tr("Garde Vaerilien")
             << QObject::tr("Ombre");
    QFile file( "out/race.xml" );
     if( !file.open( QIODevice::WriteOnly ) )
     {
         qDebug() << "argh je peux pas écrire";
     }
    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("races");
    QStringList caracName, raceGift;
    caracName << "health" << "strength" << "agility" << "reflexe" << "willpower" << "mana" << "cost";
    raceGift << "9-1" << "11-1"  << "!10-1" << "!11-1"<< "11-1+11-1"<< "9-1"<< "Élémentaire-1"
             << "11-1" << "11-1" <<  "!11-1" << "10-1" << "!11-1" << "Technologie-1" << "12-1"
             << "!11-1" << "Portail-1+9-1" << "11-1" << "10-1" << "Vol-1+11-1"
             << "Vol-1+11-1" << "Vol-1+11-1" << "11-1" << "9-1" << "12-2" << "9-1" << "Vol-1"
             << "10-1" << "Vol-1+!11-1" << "10-1" << "Discrétion / Escamotage-2";
    for(int i=0;i<raceB.size();i++)
    {
        stream.writeStartElement("race");
        stream.writeTextElement("id", _d->getFullId("race", i));
        stream.writeTextElement("name", raceName[i]);
        for(int j=0;j<raceB[i].size();j++)
        {
            stream.writeTextElement(caracName[j], QString::number(raceB[i][j]));
        }
        stream.writeStartElement("gift");
        if(raceGift[i].indexOf("+")!=-1)
        {
            stream.writeAttribute("number","2");
            QString comp1 = raceGift[i].left(raceGift[i].indexOf("+"));

            QString level = comp1.at(comp1.length()-1);
            comp1 = comp1.left(comp1.indexOf("-"));
            QString comp2 = raceGift[i].remove(0, raceGift[i].indexOf("+")+1);//+1 is for the "+"
            comp2 = comp2.left(comp2.indexOf("-"));
            QString id1= "";
            QString id2 = "";
            if(comp1.at(0)=='!')
            {
                comp1 = comp1.right(2);//TO CHANGE FOR 9 GIVES UNKONWK SKILL INSTEAD OF ANY SKILL
                qint32 idX = comp1.toInt() - 9 + 3;//-9 cf later, +3 is because non-weapon/magic/diverse is +3 more than standard
                id1 = _d->getFullId("misc", idX);
            }
            else if (comp1.at(0).isDigit())
            {
                qint32 idX = comp1.toInt() - 9;//We offset by 9 because misc@0 is unknown skill
                id1 = _d->getFullId("misc", idX);
            }
            else
            {
                id1 = _d->getFullId("skill", _d->findSkillIndex(comp1));
            }
            if(comp2.at(0)=='!')
            {
                comp2 = comp2.right(2);
                qint32 idX = comp2.toInt() - 9 + 3;//-9 cf later, +3 is because non-weapon/magic/diverse is +3 more than standard
                id2 = _d->getFullId("misc", idX);
            }
            else if (comp2.at(0).isDigit())
            {
                qint32 idX = comp2.toInt() - 9;//We offset by 9 because misc@0 is unknown skill
                if(comp2.toInt()==9)//any skill for vaerilien
                {
                    idX=7;
                }
                id2 = _d->getFullId("misc", idX);
            }
            else
            {
                id2 = _d->getFullId("skill", _d->findSkillIndex(comp2));
            }
            stream.writeTextElement("id1", id1);
            stream.writeTextElement("id2", id2);
            stream.writeTextElement("level", level);
        }
        else
        {
            stream.writeAttribute("number","1");
            QString comp=raceGift[i].left(raceGift[i].indexOf("-"));
            QString level = raceGift[i].remove(0, raceGift[i].indexOf("-")+1);

            QString id="";
            if(comp.at(0)=='!')
            {
                comp = comp.right(2);
                qint32 idX = comp.toInt() - 9 + 3;//-9 cf later, +3 is because non-weapon/magic/diverse is +3 more than standard
                id = _d->getFullId("misc", idX);
            }
            else if (comp.at(0).isDigit())
            {
                qint32 idX = comp.toInt() - 9;//We offset by 9 because misc@0 is unknown skill
                id = _d->getFullId("misc", idX);
            }
            else
            {
                id = _d->getFullId("skill", _d->findSkillIndex(comp));
            }
            stream.writeTextElement("id",id);
            stream.writeTextElement("level", level);


        }
        stream.writeEndElement();//"gift"
        stream.writeEndElement();//"race"
    }
    stream.writeEndElement(); // raceS
    stream.writeEndDocument();
    file.close();
}
damageData dDM(qint32 value, qint32 roll, qint32 bonus, bool max)
{
    damageData tmpStruct;
    tmpStruct.value=value;
    tmpStruct.roll=roll;
    tmpStruct.bonus=bonus;
    tmpStruct.max=max;
    return tmpStruct;
}
void CXmlWriter::saveDamage()
{
    QList<QList<QList<damageData> > > damageB;
    QList<damageData> headZone1, headZone2,
            stomachZone1, stomachZone2, stomachZone3, stomachZone4, stomachZone5,
            bustZone1, bustZone2, bustZone3,
            armZone1, armZone2, armZone3,
            handZone1,
            legZone1, legZone2, legZone3, legZone4;
    headZone1 << dDM(-1,1,0,true) << dDM(0, 0, 2, FALSE) <<
                 dDM(-1, 2, 0, FALSE) << dDM(10, 2, 0, FALSE);
    headZone2 << dDM(-1,2,0,TRUE) << dDM(0,0,2,FALSE) << dDM(-1,1,0, FALSE)
             << dDM(12,1,0,FALSE) << dDM(4,1,0,FALSE);

    stomachZone1 << dDM(0,0,3, FALSE) << dDM(-1, 2, 0, FALSE);
    stomachZone2 << dDM(0,0,2,FALSE) << dDM(-1, 2, 0, FALSE);
    stomachZone3 << dDM(-1,1,0,TRUE) << dDM(0,0,2,FALSE) << dDM(20,1,0,FALSE);
    stomachZone4 << dDM(0, 0, 3, FALSE) << dDM(-1, 2, 0, FALSE);
    stomachZone5 << dDM(0,0, 2, FALSE) << dDM(-1, 2, 0, FALSE) << dDM(10, 2, 0, FALSE);
    bustZone1 << dDM(-1,1,0,TRUE) << dDM(0, 0, 2, FALSE)
                << dDM(-1, 2, 0, FALSE) << dDM(8,1,0,FALSE) << dDM (6, 1, 0, FALSE);
    bustZone2 << dDM(-1, 2, 0, TRUE) << dDM(0, 0, 2, FALSE) << dDM(-1, 1, 0, FALSE) << dDM(12, 1, 0, FALSE);
    bustZone3 << dDM(0, 0, 4, FALSE) << dDM(-1, 2, 0 , FALSE) << dDM(6, 1, 0, FALSE);

    armZone1 << dDM(0,0,2,FALSE) << dDM(-1, 2, 0, FALSE) << dDM(8, 1, 0, FALSE);
    armZone2 << dDM(0, 0, 2, FALSE) << dDM(-1, 3, 0, FALSE);
    armZone3 << dDM(0, 0, 2, FALSE) << dDM(-1, 2, 0, FALSE) << dDM(8, 1, 0, FALSE) << dDM(6, 1, 0, FALSE);

    handZone1 << dDM(-1, 1, 0, TRUE) << dDM(0, 0, 1, FALSE) << dDM(-1, 1, 0, FALSE);

    legZone1 << dDM(0, 0, 2, FALSE) << dDM(-1, 2, 0, FALSE) << dDM(8, 1, 0, FALSE);
    legZone2 << dDM(0, 0, 2, FALSE) << dDM(-1, 2, 0, FALSE);
    legZone3 << dDM(0, 0, 2, FALSE) << dDM(-1, 1, 0, FALSE) << dDM(8, 1, 0, FALSE);
    legZone4 << dDM(-1, 1, 0, TRUE) << dDM(0, 0, 2, FALSE) << dDM(-1, 1, 0, FALSE)
             << dDM(6, 1, 0, FALSE) << dDM(4, 1, 0, FALSE);
    for(int i=0;i<6;i++)
    {
        QList<QList<damageData> > tmp1;
        damageB.append(tmp1);
    }
    damageB[0] << headZone1 << headZone2;
    damageB[1] << stomachZone1 << stomachZone2 << stomachZone3 << stomachZone4 << stomachZone5;
    damageB[2] << bustZone1 << bustZone2 << bustZone3;
    damageB[3] << armZone1 << armZone2 << armZone3;
    damageB[4] << handZone1 << handZone1 << handZone1;//same damage on 3 zones ! :p
    damageB[5] << legZone1 << legZone2 << legZone3 << legZone4;

    QList<QStringList> bodyZoneName;
    QStringList headZoneName, stomachZone1Name, bustZoneName, armZoneName, handZoneName, legZoneName;
    QStringList bodyPartName;
    headZoneName << "Crâne" << "Visage";
    stomachZone1Name << "Ventre" << "Côtes" << "Estomac" << "Appareil Génital" << "Colonne Vertébrale";
    bustZoneName << "Cage Thoracique" << "Poumons" << "Coeur" ;
    armZoneName << "Avant-bras" << "Bras" << "Épaule";
    handZoneName << "Poignet" << "Pouce" << "Doigts";
    legZoneName << "Cuisse" << "Tibias" << "Pieds" << "Genou";
    bodyZoneName << headZoneName << stomachZone1Name << bustZoneName << armZoneName << handZoneName << legZoneName;
    bodyPartName << "Tête" << "Ventre" << "Buste" << "Bras" << "Mains" << "Jambes";

    QFile file( "out/damage.xml" );
     if( !file.open( QIODevice::WriteOnly ) )
     {
         qDebug() << "argh je peux pas écrire";
     }
    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("damages");

    for(int i=0;i<damageB.size();i++)//Head
    {
        stream.writeStartElement("bodyPart");
        stream.writeAttribute("name", bodyPartName[i]);
        for(int j=0;j<damageB[i].size();j++)//Skull
        {

            stream.writeStartElement("bodyZone");
            stream.writeAttribute("name", bodyZoneName[i][j]);
            for(int k=0;k<damageB[i][j].size();k++)//ACTUAL damageData
            {
                stream.writeStartElement("damage");
                stream.writeTextElement("value", QString::number(damageB[i][j][k].value));
                stream.writeTextElement("roll", QString::number(damageB[i][j][k].roll));
                stream.writeTextElement("bonus", QString::number(damageB[i][j][k].bonus));
                QString max = damageB[i][j][k].max ? "true" : "false";
                stream.writeTextElement("max", max);
                stream.writeEndElement();//"damage"
            }
            stream.writeEndElement();//"bodyzone"
        }
        stream.writeEndElement();//"bodyPart"
    }


    stream.writeEndElement();//"damageS"
    stream.writeEndDocument();
    file.close();
}


void CXmlWriter::saveReligion()
{
    QList<religionPower> asept, chyrma, horwill, iitr, levilla, likeil, mebie, naax, ostrya, ranes,
            ronze, tisminefnip, vebie;
    QList<QList<religionPower > > religionB;
    QStringList godName;
    asept << religionPower { "Culte d'Asept", false, 10, 0 }
          << religionPower { "Fanatiques d'Asept", true, 15, 0 }
          << religionPower { "Maîtres d'Arts", false , 35, 0 }
          << religionPower { "Hétaïres", false , 25, 0 }
          << religionPower { "Mendiants d'Asept", true , 40, 0};

    chyrma << religionPower { "Culte de Chyrma", false, 30, 0 }
            << religionPower { "Prières de Complexion", false , 0, 11 }
           << religionPower { "Membru de Chyrma", true , 0, 12 }
           << religionPower { "Baptisé de Chyrma", false , 5, 0 }
           << religionPower { "Chevalier Élémentaire", true , 40, 0 }
           << religionPower { "Paladin de Chyrma", true , 90, 0 }
           << religionPower { "Zélé de Chyrma", true , 50, 0 }
           << religionPower { "Guérisseur de Chyrma", true , 50, 0 }
           << religionPower { "Prodige de Chyrma", true , 40, 0 }
           << religionPower { "Élus des Dieux", true , 60, 13 }
           << religionPower { "Suzerain Absolu", true , 70, 0 };

    horwill << religionPower { "Culte d'Horwill", false, 20, 0 };

    iitr    << religionPower { "Culte d'Iitr", false, 30, 0 };

    levilla << religionPower { "Culte de Levilla", false, 10, 0 }
            << religionPower { "Repos de Levilla", false , 5, 0 }
            << religionPower { "Bonne étoile", false , 10, 0 }
            << religionPower { "Voyageur-nomade", true , 15, 0 }
            << religionPower { "Héros", true , 40, 0 }
            << religionPower { "Ange Gardien", true , 75, 0 }   ;

    likeil << religionPower { "Culte de Likeil", false, 20, 0 };

    mebie << religionPower { "Culte de Mebie", false, 20, 0 };

    naax    << religionPower { "Culte de Naax", false, 10, 0 };

    ostrya << religionPower { "Culte d'Ostrya", false, 20, 0 }
            << religionPower { "Chasseurs", false , 15, 0 }
           << religionPower { "Florescence", true , 30, 0 }
           << religionPower { "Force de la Nature", true , 40, 84 }
           << religionPower { "Druide", true , 80, 0 }
           << religionPower { "Sacré d'Ostrya", true , 70, 0 };

    ranes << religionPower { "Culte de Ranes", false, 20, 0 }
            << religionPower { "Rempart de Ranes", true , 25, 0 }
          << religionPower { "Pugiliste de puissance", false , 30, 0 }
          << religionPower { "Hérault de Gloire", true , 25, 0 }
          << religionPower { "Archers tacticiens", true , 20, 0 }
          << religionPower { "Parangon de Ranes", true , 50, 0 }
          << religionPower { "Sang d'acier", true , 50, 0 }
          << religionPower { "Prêtre-artisan de Ranes", true , 20, 0 };

    ronze << religionPower { "Culte de Ronze", false, 10, 0 };

    tisminefnip << religionPower { "Culte de Tisminefnip", false, 30, 0 };

    vebie << religionPower { "Culte de Vebie", false, 20, 0 }
          << religionPower { "Siffleur de la nuit", true , 25, 0 }
          << religionPower { "Perceur de Secret", true , 25, 0 }
          << religionPower { "Aigrefin nocturne", false, 15, 0 }
          << religionPower { "Enigme des Ténèbres", false, 40, 0 }
          << religionPower { "Fils de la Magie", true , 60, 0 };
    religionB << asept << chyrma << horwill << iitr << levilla << likeil << mebie << naax << ostrya
              << ranes << ronze << tisminefnip << vebie;
    godName << "Asept" << "Chyrma" << "Horwill" << "Iitr" << "Levilla" << "Likeil" << "Mebie" << "Naax"
            << "Ostrya" << "Ranes" << "Ronze" << "Tisminefnip" << "Vebie";
    QStringList aseptC, chyrmaC, horwillC, iitrC, levillaC, likeilC, mebieC, naaxC, ostryaC, ranesC,
            ronzeC, tisminefnipC, vebieC;
    QList<QStringList> religionCompatibility;
    aseptC << "c" << "t" << "t" << "c" << "c" << "t" << "c" << "c" << "i" << "i" << "i" << "i" << "t" ;
    chyrmaC << "t" << "c" << "c" << "i" << "c" << "c" << "i" << "i" << "c" << "c" << "c" << "i" << "t" ;
    horwillC << "t" << "c" << "c" << "i" << "c" << "i" << "i" << "i" << "i" << "c" << "c" << "i" << "t" ;
    iitrC << "c" << "i" << "i" << "c" << "i" << "i" << "c" << "c" << "i" << "i" << "i" << "i" << "t" ;
    levillaC << "c" << "c" << "c" << "i" << "c" << "c" << "c" << "t" << "t" << "c" << "c" << "i" << "t" ;
    likeilC << "t" << "i" << "i" << "c" << "c" << "t" << "c" << "c" << "c" << "c" << "i" << "t"  ;
    mebieC << "c" << "i" << "i" << "c" << "c" << "t" << "c" << "c" << "c" << "c" << "i" << "i" ;
    naaxC << "c" << "i" << "i" << "c" << "t" << "c" << "c" << "c" << "i" << "i" << "i" << "i" << "t" ;
    ostryaC  << "i" << "c" << "i" << "i" << "t" << "c" << "c" << "i" << "c" << "c" << "t" << "i" << "t" ;
    ranesC << "i" << "c" << "c" << "i" << "c" << "c" << "c" << "i" << "c" << "c" << "c" << "i" << "t"  ;
    ronzeC << "i" << "c" << "c" << "i" << "c" << "c" << "c" << "i" << "t" << "c" << "c" << "t" << "c";
    tisminefnipC << "i" << "i" << "i" << "i" << "i" << "i" << "i" << "i" << "i" << "i" << "t" << "c" << "c"  ;
    vebieC << "t" << "t" << "t" << "t" << "t" << "i" << "t" << "t" << "t" << "c" << "c" << "c";
    religionCompatibility << aseptC << chyrmaC << horwillC << iitrC << levillaC << likeilC << mebieC << naaxC << ostryaC
                          << ranesC << ronzeC << tisminefnipC << vebieC;

    QFile file( "out/religion.xml" );
     if( !file.open( QIODevice::WriteOnly ) )
     {
         qDebug() << "argh je peux pas écrire";
     }
    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("religions");
    qint32 realPowerInd = 0;
    for(int i=0;i<religionB.size();i++)
    {
        stream.writeStartElement("religion");
        stream.writeTextElement("id", _d->getFullId("religion",i));
        stream.writeTextElement("name", godName[i]);
        for(int j=0;j<religionB[i].size();j++)
        {
            stream.writeStartElement("religionPower");
            stream.writeTextElement("id",_d->getFullId("religionPower",realPowerInd));
            stream.writeTextElement("name", religionB[i][j].name);
            QString cult = religionB[i][j].cult ? "true" : "false";
            stream.writeTextElement("cult",cult);
            stream.writeTextElement("cost",QString::number(religionB[i][j].cost));
            stream.writeTextElement("special", QString::number(religionB[i][j].special));
            stream.writeEndElement();//"religionPower
            realPowerInd++;
        }
        stream.writeStartElement("compatibilities");
        for(int l=0;l<religionCompatibility[i].size();l++)
        {
            stream.writeStartElement("compatibility");
            stream.writeTextElement("id", _d->getFullId("religion",l));
            stream.writeTextElement("state", religionCompatibility[i][l]);
            stream.writeEndElement();//compatibility

        }
        stream.writeEndElement();//"compatibilitieS"
        stream.writeEndElement();//"religion"
    }
    stream.writeEndElement();//"religionS"
    stream.writeEndDocument();
}


*/
