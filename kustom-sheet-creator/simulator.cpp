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
CSimulator::CSimulator()
{
    //Initialisation of random seed to have different values for qrand()
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    diceNumber = 1;
    probabilityNumber = 1;
    probabilityRoll = 10000;
    createDiceBox();
    createProbabilityBox();
    createSimulatorLayout();
    setLayout(simulatorLayout);
}
CSimulator::~CSimulator()
{
}
/*Used to roll a single roll (call multiple time if we have AdB+C, EdF+G)
 * */
qint32 CSimulator::diceRoll(diceData dice)
{
    qint32 result = 0;
    //If AdB+C We add A times a rand(1,B) and finally add C
    for(int j=0;j<dice.roll;j++)
    {
        result = result + randInt(1, dice.value);
    }
    result = result + dice.bonus;
    return result;
}

qint32 CSimulator::randInt(qint32 low, qint32 high)
{
    return qrand() % ((high + 1) - low) + low;
}

void CSimulator::createDiceBox()
{
    diceBox = new QGroupBox(tr("Lanceur de dés"));
    diceGrid = new QGridLayout;
    diceGrid->addWidget(new QLabel(tr("Nombre de lignes")),0, 0);
    diceNumberInput = new QSpinBox;
    diceNumberInput->setValue(diceNumber);
    diceNumberInput->setMaximum(10);
    connect(diceNumberInput, SIGNAL(valueChanged(int)), this, SLOT(updateDiceNumber()));
    diceGrid->addWidget(diceNumberInput,0, 1);
    diceGrid->addWidget(new QLabel(tr("Valeur du dé")), 1, 0);
    diceGrid->addWidget(new QLabel(tr("Résultat")), 1, 1);
    diceGrid->addWidget(new QLabel(tr("2d10+4,1d6+0")), 2, 0);
    diceGrid->addWidget(new QLabel(tr("12")), 2, 1);
    updateDiceNumber();
    diceBox->setLayout(diceGrid);
}
/*
 * */
void CSimulator::createProbabilityBox()
{
    probabilityBox = new QGroupBox(tr("Calculs (approximatifs) de probabilité"));
    probabilityGrid = new QGridLayout;
    probabilityGrid->addWidget(new QLabel(tr("Nombre de lignes")), 0, 0);
    probabilityNumberInput = new QSpinBox;
    probabilityNumberInput->setValue(probabilityNumber);
    probabilityNumberInput->setMaximum(10);
    connect(probabilityNumberInput, SIGNAL(valueChanged(int)), this, SLOT(updateProbabilityNumber()));
    probabilityGrid->addWidget(probabilityNumberInput, 0, 1);
    probabilityGrid->addWidget(new QLabel(tr("Valeur du premier dé")), 1, 0);
    probabilityGrid->addWidget(new QLabel(tr("Valeur du deuxième dé")),1, 1);
    probabilityGrid->addWidget(new QLabel(tr("Victoires")), 1, 2);
    probabilityGrid->addWidget(new QLabel(tr("Égalité")), 1, 3);
    probabilityGrid->addWidget(new QLabel(tr("Défaites")), 1, 4);
    probabilityGrid->addWidget(new QLabel(tr("2d10+5,4d4+2")), 2, 0);
    probabilityGrid->addWidget((new QLabel(tr("25"))), 2, 1);
    probabilityGrid->addWidget((new QLabel(tr("40,5"))), 2, 2);
    probabilityGrid->addWidget((new QLabel(tr("43.8"))), 2, 3);
    probabilityGrid->addWidget((new QLabel(tr("15,7"))), 2, 4);
    updateProbabilityNumber();
    probabilityBox->setLayout(probabilityGrid);
}
/*
 * */
void CSimulator::createSimulatorLayout()
{
    simulatorLayout = new QVBoxLayout;
    buttonDiceRoll = new QPushButton("&Lancer les dés", this);
    buttonProbabilityRoll = new QPushButton("&Lancer les simulations", this);
    connect(buttonDiceRoll, SIGNAL(clicked()), this, SLOT(updateDiceValue()));
    connect(buttonProbabilityRoll, SIGNAL(clicked()), this, SLOT(updateProbabilityValue()));
    simulatorLayout->addWidget(diceBox);
    simulatorLayout->addWidget(buttonDiceRoll);
    simulatorLayout->addWidget(probabilityBox);
    simulatorLayout->addWidget(buttonProbabilityRoll);
}
/*
 * */
void CSimulator::updateDiceValue()
{
    for(int i=0;i<diceNumber;i++)
    {
        QString rawText=diceI[i].value->displayText();
        rawText.simplified();
        rawText.remove(" ");
        if(!isDice(rawText))
        {
            QMessageBox::warning(this, "Erreur lors de l'entrée des dés", "Voir exemple pour une valeur correcte des dés.");
            return;
        }
        qint32 diceResult=0;
        qint32 plusNumber=rawText.count("+");
        qint32 commaNumber=rawText.count(",");//1 comma = 2 dice, etc
        //We (can) have multiple dice for one roll (eg 2d10+4 + 3d6+2)
        for(int j=0;j<commaNumber+1;j++)
        {

            if(diceBoxD[i].size() < commaNumber +1)
            {//We need to fill the diceBoxD[i][j] cause it doesn't exist atm
                diceData tmpData;
                tmpData.value=0;
                tmpData.roll=0;
                tmpData.bonus=0;
                diceBoxD[i] << tmpData;
            }
            if(plusNumber==0)
            {
                diceBoxD[i][j].value=0;
                diceBoxD[i][j].roll=0;
                diceBoxD[i][j].bonus=rawText.toInt();
                diceResult = diceResult + diceRoll(diceBoxD[i][j]);
                continue;
            }
            qint32 d=rawText.indexOf("d");
            diceBoxD[i][j].roll=rawText.left(d).toInt();
            rawText.remove(0,rawText.left(d).size()+1);//We remove Ad at the beginning

            qint32 plus=rawText.indexOf("+");
            diceBoxD[i][j].value=rawText.left(plus).toInt();
            rawText.remove(0,rawText.left(plus).size()+1);//We remove B+
            qint32 comma=rawText.indexOf(",");
            diceBoxD[i][j].bonus=rawText.left(comma).toInt();

            rawText.remove(0, rawText.left(comma).size()+1);//We remove C,
            //The dice is filled, do the roll
            diceResult = diceResult + diceRoll(diceBoxD[i][j]);
        }
    diceI[i].result->setValue(diceResult);
    }
}
/*
 * */
void CSimulator::updateProbabilityValue()
{
    for(int i=0;i<probabilityNumber;i++)
    {
        QString rawText=probabilityI[i].value1->displayText();
        rawText.simplified();
        rawText.remove(" ");
        if(!isDice(rawText))
        {
            QMessageBox::warning(this, "Erreur lors de l'entrée des dés", "Voir exemple pour une valeur correcte des dés.");
            return;
        }
        qint32 plusNumber=rawText.count("+");
        qint32 commaNumber=rawText.count(",");//1 comma = 2 probability, etc
        for (int j=commaNumber+1;j<probabilityBoxD1[i].size();j++)
        {
            probabilityBoxD1[i][j].value=0;
            probabilityBoxD1[i][j].roll=0;
            probabilityBoxD1[i][j].bonus=0;
        }
        //We (can) have multiple probability for one roll (eg 2d10+4 + 3d6+2)

        for(int j=0;j<commaNumber+1;j++)
        {
            //If we have a non-roll value (eg 15)
            if(plusNumber==0)
            {
                probabilityBoxD1[i][0].value=0;
                probabilityBoxD1[i][0].roll=0;
                probabilityBoxD1[i][0].bonus=rawText.toInt();
                continue;
            }
            if(probabilityBoxD1[i].size() < commaNumber +1)
            {//We need to fill the probabilityBoxD1[i][j] cause it doesn't exist atm
                diceData tmpData;
                tmpData.value=0;
                tmpData.roll=0;
                tmpData.bonus=0;
                probabilityBoxD1[i] << tmpData;
            }
            qint32 d=rawText.indexOf("d");
            probabilityBoxD1[i][j].roll=rawText.left(d).toInt();
            rawText.remove(0,rawText.left(d).size()+1);//We remove Ad at the beginning

            qint32 plus=rawText.indexOf("+");
            probabilityBoxD1[i][j].value=rawText.left(plus).toInt();

            rawText.remove(0,rawText.left(plus).size()+1);//We remove B+
            qint32 comma=rawText.indexOf(",");
            probabilityBoxD1[i][j].bonus=rawText.left(comma).toInt();

            rawText.remove(0, rawText.left(comma).size()+1);//We remove C,
        }


        //Now 2nd proba
        rawText="";
        rawText=probabilityI[i].value2->displayText();
        rawText.simplified();
        rawText.remove(" ");
        if(!isDice(rawText))
        {
            QMessageBox::warning(this, "Erreur lors de l'entrée des dés", "Voir exemple pour une valeur correcte des dés.");
            return;
        }
        plusNumber=rawText.count("+");
        commaNumber=rawText.count(",");//1 comma = 2 probability, etc
        for (int j=commaNumber+1;j<probabilityBoxD2[i].size();j++)
        {
            probabilityBoxD2[i][j].value=0;
            probabilityBoxD2[i][j].roll=0;
            probabilityBoxD2[i][j].bonus=0;
        }
        //We (can) have multiple probability for one roll (eg 2d10+4 + 3d6+2)
        for(int j=0;j<commaNumber+1;j++)
        {
            if(plusNumber==0)
            {
                probabilityBoxD2[i][0].value=0;
                probabilityBoxD2[i][0].roll=0;
                probabilityBoxD2[i][0].bonus=rawText.toInt();
                continue;
            }
            if(probabilityBoxD2[i].size() < commaNumber +1)
            {//We need to fill the probabilityBoxD1[i][j] cause it doesn't exist atm
                diceData tmpData;
                tmpData.value=0;
                tmpData.roll=0;
                tmpData.bonus=0;
                probabilityBoxD2[i] << tmpData;
            }
            qint32 d=rawText.indexOf("d");
            probabilityBoxD2[i][j].roll=rawText.left(d).toInt();
            rawText.remove(0,rawText.left(d).size()+1);//We remove Ad at the beginning

            qint32 plus=rawText.indexOf("+");
            probabilityBoxD2[i][j].value=rawText.left(plus).toInt();

            rawText.remove(0,rawText.left(plus).size()+1);//We remove B+
            qint32 comma=rawText.indexOf(",");
            probabilityBoxD2[i][j].bonus=rawText.left(comma).toInt();

            rawText.remove(0, rawText.left(comma).size()+1);//We remove C,
        }



    float win=0;
    float draw=0;
    float losse=0;
    //We do the 10'000 rolls !!!
    for(int j=0;j<probabilityRoll;j++)
    {
        qint32 result1=0;
        qint32 result2=0;
        for(int k=0;k<probabilityBoxD1[i].size();k++)
        {
            result1=result1 + diceRoll(probabilityBoxD1[i][k]);
        }
        for(int k=0;k<probabilityBoxD2[i].size();k++)
        {
            result2=result2 + diceRoll(probabilityBoxD2[i][k]);
        }
        if (result1 > result2)
        {
            win++;
        }
        else if(result1 < result2)
        {
            losse++;
        }
        else
        {
            draw++;
        }
    }
    probabilityI[i].win->setValue(win*100/probabilityRoll);
    probabilityI[i].draw->setValue(draw*100/probabilityRoll);
    probabilityI[i].losse->setValue(losse*100/probabilityRoll);
    }
}
/*
 * */
void CSimulator::updateDiceNumber()
{
    diceNumber=diceNumberInput->value();
    if(diceNumber>diceI.size())//We had more rows of things
    {
        qint32 tmpISize=diceI.size();
        for(int i = 0;i<diceNumber-diceI.size();i++)
        {
            //Fill the input
            diceInput tmpStruct;
            tmpStruct.value=new QLineEdit;
            tmpStruct.result=new QSpinBox;
            tmpStruct.result->setReadOnly(true);
            diceI.append(tmpStruct);
            qint32 tmpRowCount=diceGrid->rowCount();
            diceGrid->addWidget(diceI[tmpISize+i].value,tmpRowCount,0);
            diceGrid->addWidget(diceI[tmpISize+i].result,tmpRowCount, 1);
            //Fill the Data (0 everywhere by default)
            diceData tmpData;
            tmpData.value=0;
            tmpData.roll=0;
            tmpData.bonus=0;
            QList<diceData> tmpList;
            tmpList << tmpData;
            diceBoxD << tmpList;
        }
    }

    if(diceNumber<diceI.size())
    {
        QGridLayout* tmpGrid;
        tmpGrid=new QGridLayout;
        for(int i=0;i<diceNumber+3;i++)//We add 3 because of the first 3 lines (and line begins at 1)
        {

            tmpGrid->addWidget(diceGrid->itemAtPosition(i,0)->widget(),
                                   i, 0);
            tmpGrid->addWidget(diceGrid->itemAtPosition(i,1)->widget(),
                                   i, 1);
        }
        //We recreated a new grid ; now we destroy all children from the old one then the grid itself
        //then copy the new one to the old one and update the layout.
        QLayoutItem *child;
        while ((child = diceGrid->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
        }
        delete diceGrid;
        diceGrid=tmpGrid;
        diceBox->setLayout(diceGrid);
        //Finally we need to remove the actuals widgets (we just removed the layout before) & data who goes along
        for(int i=diceI.size();i>diceNumber;i--)
        {
            diceI.removeLast();
            diceBoxD.removeLast();
        }
    }
}
/*
 * */
void CSimulator::updateProbabilityNumber()
{
    probabilityNumber=probabilityNumberInput->value();
    if(probabilityNumber>probabilityI.size()) //We had more rows of things
    {
        qint32 tmpISize=probabilityI.size();//We remove
        for(int i = 0;i<probabilityNumber-probabilityI.size();i++)
        {
            probabilityInput tmpStruct;
            tmpStruct.value1=new QLineEdit;
            tmpStruct.value2=new QLineEdit;
            tmpStruct.win=new QDoubleSpinBox;
            tmpStruct.draw=new QDoubleSpinBox;
            tmpStruct.losse=new QDoubleSpinBox;
            tmpStruct.win->setReadOnly(true);
            tmpStruct.draw->setReadOnly(true);
            tmpStruct.losse->setReadOnly(true);
            //To be able to display 100%
            tmpStruct.win->setMaximum(101);
            tmpStruct.draw->setMaximum(101);
            tmpStruct.losse->setMaximum(101);
            probabilityI.append(tmpStruct);
            qint32 tmpRowCount=probabilityGrid->rowCount();
            probabilityGrid->addWidget(probabilityI[tmpISize+i].value1, tmpRowCount, 0);
            probabilityGrid->addWidget(probabilityI[tmpISize+i].value2, tmpRowCount, 1);
            probabilityGrid->addWidget(probabilityI[tmpISize+i].win, tmpRowCount, 2);
            probabilityGrid->addWidget(probabilityI[tmpISize+i].draw, tmpRowCount, 3);
            probabilityGrid->addWidget(probabilityI[tmpISize+i].losse, tmpRowCount, 4);
            //Fill the Data (0 everywhere by default), we need 2 sets of data (cause, y'know, dice vs dice)
            diceData tmpData;
            tmpData.value=0;
            tmpData.roll=0;
            tmpData.bonus=0;
            QList<diceData> tmpList;
            tmpList << tmpData << tmpData;
            probabilityBoxD1 << tmpList;
            probabilityBoxD2 << tmpList;
        }
    }

    if(probabilityNumber<probabilityI.size())
    {
        QGridLayout* tmpGrid;
        tmpGrid=new QGridLayout;
        for(int i=0;i<probabilityNumber+3;i++)//We add 4 because of the first 3 lines (and line begins at 1)
        {
            for(int j=0;j<probabilityGrid->columnCount();j++)
            {
                if(i>1 || j<2)//UGLY HACK NEED TO FIX LOLOLOL
                {
                tmpGrid->addWidget(probabilityGrid->itemAtPosition(i,j)->widget(),i, j);
                }
            }
        }
        //We recreated a new grid ; now we destroy all children from the old one then the grid itself
        //then copy the new one to the old one and update the layout.
        QLayoutItem *child;
        while ((child = probabilityGrid->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
        }
        delete probabilityGrid;
        probabilityGrid=tmpGrid;
        probabilityBox->setLayout(probabilityGrid);
        //Finally we need to remove the actuals widgets (we just removed the layout before) & data who goes along
        for(int i=probabilityI.size();i>probabilityNumber;i--)
        {
            probabilityI.removeLast();
            probabilityBoxD1.removeLast();
        }
    }
}

bool CSimulator::isDice(QString rawText)
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
