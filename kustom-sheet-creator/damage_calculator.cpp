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
#include "main_window.h"
#include "data.h"
#include "generator.h"
#include "simulator.h"
#include "signal.h"
#include "damage_calculator.h"

CDamageCalculator::CDamageCalculator(CData *data)
{
    d = data;
    createPersonalBox();
    createBodyPartBox();
    createDamageCalculatorLayout();
    setLayout(damageCalculatorLayout);
}
/*Used to roll a single roll (call multiple time if we have AdB+C, EdF+G), handle maxed dice

* */
qint32 CDamageCalculator::diceRoll(damageData dice)
{
   qint32 result = 0;
   //If AdB+C We add A times a rand(1,B) and finally add C
   for(int j=0;j<dice.roll;j++)
   {
       if(dice.max)
       {
           result = result + dice.value;
       }
       else
       {
           result = result + randInt(1, dice.value);
       }
   }
   result = result + dice.bonus;
   return result;
}

qint32 CDamageCalculator::randInt(qint32 low, qint32 high)
{
   return qrand() % ((high + 1) - low) + low;
}

/*
 * */
void CDamageCalculator::createPersonalBox()
{
    personalBox = new QGroupBox(tr("Dégâts de votre personnage"));
    personalGrid = new QGridLayout;
    damageI = new QLineEdit;
    damageBonusI = new QSpinBox;
    damageSupI = new QLineEdit;
    personalGrid->addWidget(new QLabel(tr("Dégât de l'arme")), 0, 0);
    personalGrid->addWidget(damageI, 0, 1);
    personalGrid->addWidget(new QLabel(tr("Bonus de dégâts")), 1, 0);
    personalGrid->addWidget(damageBonusI, 1, 1);

    personalGrid->addWidget(new QLabel(tr("Dégâts supplémentaires")), 2, 0);
    personalGrid->addWidget(damageSupI, 2, 1);

    damageData tmpStruct;
    tmpStruct.value=0;
    tmpStruct.roll=0;
    tmpStruct.bonus=0;
    tmpStruct.max=false;
    damageUser <<tmpStruct;
    damageUserSup << tmpStruct;
    personalBox->setLayout(personalGrid);
}

/*
 * */
void CDamageCalculator::createBodyPartBox()
{

    //We do that just to have the correct template in damageD (we'll change every value of it anyway)
    damageD=d->getDamageB();
    for(int i=0;i<d->getBodyPartName().size();i++)
    {
        bodyPartBox.append(new QGroupBox(d->getBodyPartName()[i]));
        bodyPartGrid.append(new QGridLayout);
        QList<bodyZoneInput> tmpList;
        bodyI.append(tmpList);
        bodyPartGrid[i]->addWidget(new QLabel(tr("Zone")), 0,0);
        bodyPartGrid[i]->addWidget(new QLabel(tr("Dégâts")),0,1);
        bodyPartGrid[i]->addWidget(new QLabel(tr("Min-Max[Moy]")),0,2);
        //We now add all zone
        for(int j=0;j<d->getBodyZoneName()[i].size();j++)
        {
            bodyZoneInput tmpStruct;
            tmpStruct.name=new QLabel(d->getBodyZoneName()[i][j]);
            tmpStruct.value=new QSpinBox;
            tmpStruct.value->setReadOnly(true);
            tmpStruct.stat=new QLineEdit;
            tmpStruct.stat->setReadOnly(true);
            bodyI[i].append(tmpStruct);
            bodyPartGrid[i]->addWidget(bodyI[i][j].name, 1+j, 0);
            bodyPartGrid[i]->addWidget(bodyI[i][j].value, 1+j, 1);
            bodyPartGrid[i]->addWidget(bodyI[i][j].stat, 1+j, 2);

        }
        bodyPartBox[i]->setLayout(bodyPartGrid[i]);
    }
}

/*


 * */
void CDamageCalculator::createDamageCalculatorLayout()
{
    damageCalculatorLayout = new QGridLayout;
    buttonCalculate = new QPushButton("&Calculer les dégâts", this);
    connect(buttonCalculate, SIGNAL(clicked()), this, SLOT(calculateDamage()));
    damageCalculatorLayout->addWidget(personalBox,0,0, 1, 3);
    for(int i=0;i<bodyPartBox.size();i++)
    {
        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        if(i%2==0)
        {
            damageCalculatorLayout->addWidget(bodyPartBox[i], 1+i/2, 0);
            damageCalculatorLayout->addWidget(line, 1+i/2,1);
        }
        else
        {
            damageCalculatorLayout->addWidget(bodyPartBox[i], 1+(i-1)/2, 2);
        }

    }
    damageCalculatorLayout->addWidget(buttonCalculate,4,0,1, 3);
}

/*
 * */
void CDamageCalculator::calculateDamage()
{
    if(!updateDamageData())
    {
        return;
    }
    //All data OK, let's go !
    for(int i=0;i<damageD.size();i++)//body part (head, stoamch ...)
    {
        for(int j=0;j<damageD[i].size();j++)//(body zone (skull, face)
        {
            qint32 result = 0;
            qint32 min = 0;
            qint32 max = 0;
            float mean = 0;
            for(int k=0;k<damageD[i][j].size();k++) //Different damage roll
            {
                result = result + diceRoll(damageD[i][j][k]);
                damageData tmpMax = damageD[i][j][k];
                if(tmpMax.max)
                {
                    min = min + diceRoll(tmpMax);
                }
                else
                {
                    damageData tmpMin = damageD[i][j][k];
                    tmpMin.value=1;
                    min = min + diceRoll(tmpMin);
                }
                tmpMax.max=true;
                max = max + diceRoll(tmpMax);


                //If damage is maxed, we add the full dice, else we had dice +1/2 (average of the dice)
                if(damageD[i][j][k].max)
                {
                    mean = mean + damageD[i][j][k].value * damageD[i][j][k].roll + damageD[i][j][k].bonus;
                }
                else
                {
                    mean = mean +  damageD[i][j][k].roll * (damageD[i][j][k].value+1)/2 + damageD[i][j][k].bonus;
                }
            }
            //Different for cause size will probably differ
            for(int k=0;k<damageUserSup.size();k++)
            {
                result = result +diceRoll(damageUserSup[k]);
                mean = mean + damageUserSup[k].roll * (damageUserSup[k].value+1)/2 + damageUserSup[k].bonus;
                damageData tmpMax = damageUserSup[k];
                tmpMax.max=true;
                max = max + diceRoll(tmpMax);

                tmpMax.value=1;

                min = min + diceRoll(tmpMax);

            }
            bodyI[i][j].value->setValue(result);
            bodyI[i][j].stat->setText(QString::number(min)+ "-" + QString::number(max) + "[" + QString::number(mean)+"]");
        }
    }

}

/*
 * */
bool CDamageCalculator::updateDamageData()
{
    if(!updateDamageUser())
    {
        return false;
    }

    for(int i=0;i<d->getDamageB().size();i++) //different body Part (head, stomach ...)
    {
        for(int j=0;j<d->getDamageB()[i].size();j++) //Different body zone (skull, face)
        {
            damageD[i][j].clear();
            for (int k=0;k<d->getDamageB()[i][j].size();k++) //Different damage roll for one zone
            {
                for(int l=0;l<damageUser.size();l++)
                {
                    damageData tmpD;
                    tmpD.bonus=d->getDamageB()[i][j][k].bonus*damageUser[l].bonus;
                    tmpD.max=d->getDamageB()[i][j][k].max;
                    if(d->getDamageB()[i][j][k].value==-1)
                    {//It's a maxed / normal  WEAPON dice
                        tmpD.value=damageUser[l].value;
                        tmpD.roll=d->getDamageB()[i][j][k].roll*damageUser[l].roll;
                    }
                    else
                    {//Either bonus or additional dice
                        tmpD.value=d->getDamageB()[i][j][k].value;
                        tmpD.roll=d->getDamageB()[i][j][k].roll;
                    }
                    damageD[i][j]<<tmpD;
                }
            }
        }
    }
    return true;
}

/*
 * */
bool CDamageCalculator::updateDamageUser()
{
    //The weapon damage
    QString rawText=damageI->displayText();
    rawText.simplified();
    rawText.remove(" ");
    qint32 commaNumber=rawText.count(",");//1 comma = 2 dice, etc
    if(!isDice(rawText))
    {
        QMessageBox::warning(this, "Erreur lors de l'entrée des dés", "Les dégâts de l'arme sont incorrects (voir exemples du simulateur de dés)");
        return false;
    }
    damageUser.clear();
    for(int i=0;i<commaNumber+1;i++)
    {
        if(damageUser.size() < commaNumber +1)
        {
            damageData tmpData;
            tmpData.value=0;
            tmpData.roll=0;
            tmpData.bonus=0;
            tmpData.max=false;
            damageUser << tmpData;
        }
        qint32 plusNumber=rawText.count("+");
        //We (can) have multiple dice for one roll (eg 2d10+4 + 3d6+2)
        if(plusNumber==0)
        {
            damageUser[i].value=0;
            damageUser[i].roll=0;
            damageUser[i].bonus=rawText.toInt();
            continue;
        }
        qint32 d2=rawText.indexOf("d");
        damageUser[i].roll=rawText.left(d2).toInt();
        rawText.remove(0,rawText.left(d2).size()+1);//We remove Ad at the beginning

        qint32 plus=rawText.indexOf("+");
        damageUser[i].value=rawText.left(plus).toInt();
        rawText.remove(0,rawText.left(plus).size()+1);//We remove B+
        qint32 comma=rawText.indexOf(",");
        damageUser[i].bonus=rawText.left(comma).toInt();
        rawText.remove(0, rawText.left(comma).size()+1);//We remove C,
        //Strength bonus + other thing (arme de prédilection ...)
        if(i==0)
        {
            damageUser[i].bonus=damageUser[i].bonus + damageBonusI->value();
        }
    }
    //additionnal damage (coup brutal for example)
    rawText=damageSupI->displayText();
    rawText.simplified();
    rawText.remove(" ");
    commaNumber=rawText.count(",");//1 comma = 2 dice, etc
    if(rawText=="")//No additionnal damage, stop here
    {
        damageUserSup[0].value=0;
        damageUserSup[0].roll=0;
        damageUserSup[0].bonus=0;
        return true;
    }
    if(!isDice(rawText))
    {
        QMessageBox::warning(this, "Erreur lors de l'entrée des dés", "Les dégâts supplémentaires sont incorrects (voir exemples du simulateur de dés)");
        return false;
    }
    for(int i=0;i<commaNumber+1;i++)
    {
        qint32 plusNumber=rawText.count("+");
        //We (can) have multiple dice for one roll (eg 2d10+4 + 3d6+2)
        if(plusNumber==0)
        {
            damageUserSup[0].value=0;
            damageUserSup[0].roll=0;
            damageUserSup[0].bonus=rawText.toInt();
            return true;
        }
        if(damageUserSup.size() < i+1)
        {
            damageData tmpStruct;
            tmpStruct.bonus=0;
            tmpStruct.max=false;
            tmpStruct.roll=0;
            tmpStruct.value=0;
            damageUserSup << tmpStruct;
        }
        if(damageUserSup.size() > commaNumber + 1)
        {
            damageUserSup.removeLast();
        }
        qint32 d2=rawText.indexOf("d");
        damageUserSup[i].roll=rawText.left(d2).toInt();
        rawText.remove(0,rawText.left(d2).size()+1);//We remove Ad at the beginning

        qint32 plus=rawText.indexOf("+");
        damageUserSup[i].value=rawText.left(plus).toInt();
        rawText.remove(0,rawText.left(plus).size()+1);//We remove B+
        qint32 comma=rawText.indexOf(",");
        damageUserSup[i].bonus=rawText.left(comma).toInt();
        rawText.remove(0, rawText.left(comma).size()+1);//We remove C,
    }
    return true;
}
/*
 * */
bool CDamageCalculator::isDice(QString rawText)
{
    qint32 commaNumber=rawText.count(",");//1 comma = 2 dice, etc
    qint32 plusNumber=rawText.count("+");
    bool ok;
    if(plusNumber==0)//We might have a single constant
    {
        rawText.toInt(&ok);
        return ok;
    }
    //We check for every diceRoll if the conversion is doing good
    for(int j=0;j<commaNumber+1;j++)
    {
        qint32 d=rawText.indexOf("d");
        rawText.left(d).toInt(&ok);
        if(!ok)
        {
            return ok;
        }
        rawText.remove(0,rawText.left(d).size()+1);//We remove Ad at the beginning

        qint32 plus=rawText.indexOf("+");
        rawText.left(plus).toInt(&ok);
        if(!ok)
        {
            return ok;
        }
        rawText.remove(0,rawText.left(plus).size()+1);//We remove B+
        qint32 comma=rawText.indexOf(",");
        rawText.left(comma).toInt(&ok);
        if(!ok)
        {
            return ok;
        }
        rawText.remove(0, rawText.left(comma).size()+1);//We remove C,

    }
    return true;
}
