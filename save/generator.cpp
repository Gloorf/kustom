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
#include "main_window.h"
#include "data.h"
#include "generator.h"
#include "odt_writer.h"
#include "dialog.h"
CGenerator::CGenerator(CData *data)
{
    d=data;
    selectedRaceIndex=0;
    createPersonalBox();
    createSkillBox();
    createCaracBox();
    createGeneratorLayout();
    setLayout(generatorLayout);
    updateCarac();
    //TODO : put it somewhere better
    /*
    for(int i=0;i<d->getReligionB().size();i++)
    {
        QList<bool> tmpList;
        religionPowerChoice << tmpList;
        for(int j=0;j<d->getReligionB()[i]->size();j++)
        {
            religionPowerChoice[i] << false;
        }
    }
    */
}


/* Struct of this widget :
 * -we will have a skillObjectB QList of CSkill object, with every skill (and so every perk), generated with odt_reader (done for skill, need perk now)
 * -same as calculator, skillI for the input
 * every skill will be in a QGroupBox (we will have a QList of QGroupBox for the layout)
 * the data will be a CSkill object too (but only the perk we have atm)
 * */

void CGenerator::createPersonalBox()
{
    personalGrid = new QGridLayout;
    personalBox = new QGroupBox("Infos générales");
    charName = new QLineEdit;
    raceI = new QComboBox;
    pointValue = new QSpinBox;
    pointValue->setMaximum(10000);
    pointValue->setReadOnly(true);
    advantageValue = new QSpinBox;
    advantageValue->setMaximum(10000);
    usedFont= QFont(d->getSettings()->value("baseFont").value<QString>(), 12);
    fontName = new QLineEdit;
    fontName->setReadOnly(true);
    if(!usedFont.exactMatch())
    {
        usedFont.setFamily(d->getSettings()->value("fallbackFont").value<QString>());
    }
    fontName->setText(usedFont.family());

    buttonFont = new QPushButton("Changer la police");
    for (int i=0; i<d->getRaceB().size();++i)
    {
        raceI->addItem(d->getRaceB()[i]->getName());
    }
    connect(raceI, SIGNAL(currentIndexChanged(int)), this, SLOT(updateCarac()));
    connect(advantageValue, SIGNAL(valueChanged(int)), this, SLOT(updateSkill()));
    connect(buttonFont, SIGNAL(clicked()), this, SLOT(updateFont()));
    personalGrid->addWidget(new QLabel(tr("Nom :")),0,0);
    personalGrid->addWidget(new QLabel(tr("Votre race")),1,0);
    personalGrid->addWidget(new QLabel(tr("Points supplémentaires")),2,0);
    personalGrid->itemAtPosition(2,0)->widget()->setToolTip("Points non inclus dans le calculateur (avantages principalement");
    personalGrid->addWidget(new QLabel(tr("Points Kustom")),3,0);
    personalGrid->itemAtPosition(3,0)->widget()->setToolTip("Valeur calculée en K pour votre personnage");
    personalGrid->addWidget(new QLabel(tr("Police ")),4,0);
    personalGrid->addWidget(charName,0,1);
    personalGrid->addWidget(raceI,1,1);
    personalGrid->addWidget(advantageValue,2,1);
    personalGrid->addWidget(pointValue,3,1);
    personalGrid->addWidget(fontName,4,1);
    personalGrid->addWidget(buttonFont, 5,0, 1, -1);

    personalBox->setLayout(personalGrid);
}

void CGenerator::createSkillBox()
{
    skillGrid = new QGridLayout;
    skillBox = new QGroupBox("Compétences");
    skillBox->setLayout(skillGrid);
}

void CGenerator::createCaracBox()
{
    caracGrid = new QGridLayout;
    caracBox = new QGroupBox("Caractéristiques");
    caracGrid->addWidget(new QLabel(tr("Personnage")),0, 1);
    caracGrid->addWidget(new QLabel(tr("Base (de la race)")),0,2);


    for (int i=0; i < d->getCaracL().size() ; ++i)
    {
        QSpinBox *tmpSpinI, *tmpSpinB;
        tmpSpinI = new QSpinBox;
        tmpSpinB = new QSpinBox;
        tmpSpinI->setMaximum(1000);
        tmpSpinB->setMaximum(1000);
        caracI.append(tmpSpinI);
        caracB.append(tmpSpinB);
        caracB[i]->setReadOnly(true);

        caracGrid->addWidget(d->getCaracL()[i], i + 1, 0);
        caracGrid->addWidget(caracI[i], i + 1, 1);
        caracGrid->addWidget(caracB[i], i + 1, 2);
        connect(caracI[i], SIGNAL(valueChanged(int)), this, SLOT(updateSkill()));
    }
    caracBox->setLayout(caracGrid);

}

void CGenerator::createGeneratorLayout()
{
    generatorLayout = new QVBoxLayout;
    topLayout = new QHBoxLayout;
    topBox = new QGroupBox("");
    topLayout->addWidget(personalBox,0,Qt::AlignTop);
    topLayout->addWidget(caracBox,0,Qt::AlignTop);
    topBox->setLayout(topLayout);

    buttonLayout = new QHBoxLayout;
    buttonBox = new QGroupBox("");

    buttonAddSkill = new QPushButton("Ajouter une compétence");
    buttonRemoveSkill = new QPushButton("Supprimer une compétence");
    buttonInfoSkill = new QPushButton("Info sur une compétence");
    buttonGenerate = new QPushButton("Générer la fiche");
    buttonReset = new QPushButton("Remettre à zéro la fiche");
    buttonReligion = new QPushButton("Pouvoirs de religion");
    connect(buttonAddSkill, SIGNAL(clicked()), this, SLOT(addSkillDialog()));
    connect(buttonRemoveSkill, SIGNAL(clicked()), this, SLOT(removeSkillDialog()));
    connect(buttonReligion, SIGNAL(clicked()), this, SLOT(religionDialog()));
    connect(buttonInfoSkill, SIGNAL(clicked()), this, SLOT(infoSkillDialog()));
    connect(buttonGenerate, SIGNAL(clicked()), this, SLOT(generateSheet()));
    connect(buttonReset, SIGNAL(clicked()), this, SLOT(resetSheet()));
    buttonLayout->addWidget(buttonAddSkill);
    buttonLayout->addWidget(buttonRemoveSkill);
    buttonLayout->addWidget(buttonReligion);
    buttonLayout->addWidget(buttonInfoSkill);
    buttonLayout->addWidget(buttonReset);
    buttonLayout->addWidget(buttonGenerate);

    buttonBox->setLayout(buttonLayout);
    skillScrollArea = new CVerticalScrollArea(skillBox);
    generatorLayout->addWidget(topBox);
    generatorLayout->addWidget(skillScrollArea);
    generatorLayout->addWidget(buttonBox);
}



void CGenerator::addSkillToLayout()
{

    //First we create the CSkill to put into skillD
    QString tmpName = _dialogNewSkill->tmpInput.name->currentText();
    qint32 realIndex=d->findSkillIndex(tmpName);//Index in skillBL

    QString special = _dialogNewSkill->tmpInput.special->currentIndex()==0 ? "n" : (_dialogNewSkill->tmpInput.special->currentIndex()==1 ? "t" : "g");
    CSkill *tmpSkill;
    tmpSkill = new CSkill;
    tmpSkill->setParam("name", _dialogNewSkill->tmpInput.name->currentText());
    tmpSkill->setParam("speed", d->getSkillB()[realIndex]->getParam("speed"));
    tmpSkill->setParam("type", d->getSkillB()[realIndex]->getParam("type"));
    tmpSkill->setParam("special", special);
    tmpSkill->setParam("level", dialogNewSkill->tmpInput.level->value());
    skillD.append(tmpSkill);
    //The skillInput is the same except we need to add the perk

    skillI.append(_dialogNewSkill->tmpInput);
    QGroupBox *tmpBox;
    QGridLayout *tmpLayout = new QGridLayout;
    tmpBox = newSkillBox(tmpLayout, skillI[skillI.size()-1]);
    skillInputBox.append(tmpBox);
    skillInputLayout.append(tmpLayout);
    qint32 skillNumber=skillInputBox.size() -1;

    if(skillNumber%2==0)
    {
        skillGrid->addWidget(skillInputBox[skillNumber],skillNumber/2,0,1,1,Qt::AlignTop);
    }
    else
    {
        skillGrid->addWidget(skillInputBox[skillNumber],(skillNumber-1)/2,1,1,1,Qt::AlignTop);
    }
    _dialogNewSkill->close();
    updateSkill();
}

/*
 * return a QGroupBox to add in skillInputBox
 * Add a Layout to skillInputLayout, a box to skillInputBox, and the box itself to skillGrid
 * */
QGroupBox* CGenerator::newSkillBox(QGridLayout *layoutOutput, skillInput input)
{

    layoutOutput->addWidget(new QLabel(tr("Nom")), 0,0);
    layoutOutput->addWidget(new QLabel(tr("Niveau")),0,1);
    layoutOutput->addWidget(new QLabel(tr("Spécial")),0,2);
    layoutOutput->addWidget(new QLabel(tr("Vitesse")),0,3);
    layoutOutput->addWidget(new QLabel(tr("Type")),0,4);
    layoutOutput->addWidget(input.name,1,0);
    layoutOutput->addWidget(input.level,1,1);
    layoutOutput->addWidget(input.special,1,2);
    layoutOutput->addWidget(input.speed,1,3);
    layoutOutput->addWidget(input.type,1,4);

    connect(input.level, SIGNAL(valueChanged(int)), this, SLOT(updateSkill()));
    connect(input.special, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSkill()));
    connect(input.name, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSkill()));
    QGroupBox *output;
    output=new QGroupBox("");
    output->setLayout(layoutOutput);
    return output;
}




void CGenerator::removeSkill()
{
    //We recreate like everything cuz no choice
    qint32 curInd = _dialogRemoveSkill->nameI->currentIndex();
    removeSkillAtInd(curInd);
    _dialogRemoveSkill->close();
    updateSkill();
}
/*
 * */
void CGenerator::removeSkillAtInd(qint32 index)
{
    QList<skillInput> tmpSkillI;
    QList<CSkill *> tmpSkillD;
    QList<QGridLayout *> tmpSkillInputLayout;
    QList<QGroupBox *> tmpSkillInputBox;
    QList<bool> tmpPerkNumberOkay;
    for(int i=0;i<skillD.size();i++)
    {
        if(i!=index)
        {
            tmpSkillI.append(skillI[i]);
            tmpSkillD.append(skillD[i]);
            tmpPerkNumberOkay.append(perkNumberOkay[i]);
            QGroupBox* tmpBox=new QGroupBox;
            QGridLayout* tmpLayout = new QGridLayout;
            tmpBox=newSkillBox(tmpLayout, tmpSkillI.last());

            for(int j=0;j<tmpSkillI.last().perkLevel.size();j++)
            {
                tmpLayout->addWidget(tmpSkillI.last().perkList[j],2+j,0);
                tmpLayout->addWidget(tmpSkillI.last().perkLevel[j],2+j,1);
                tmpLayout->addWidget(tmpSkillI.last().perkDesc[j],2+j,2,1,-1);
            }
            tmpSkillInputLayout.append(tmpLayout);
            tmpSkillInputBox.append(tmpBox);
        }
    }
    skillD=tmpSkillD;
    skillI=tmpSkillI;
    perkNumberOkay=tmpPerkNumberOkay;
    skillInputLayout=tmpSkillInputLayout;
    skillInputBox=tmpSkillInputBox;
    QLayoutItem *child;
    //Clean skillGrid
    while ((child = skillGrid->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }
    QGridLayout* tmpGrid = new QGridLayout;
    for(int i=0;i<skillInputBox.size();i++)
    {
        if(i%2==0)
        {
            tmpGrid->addWidget(skillInputBox[i],i/2,0,1,1,Qt::AlignTop);
        }
        else
        {
            tmpGrid->addWidget(skillInputBox[i],(i-1)/2,1,1,1,Qt::AlignTop);
        }
    }
    delete skillGrid;
    skillGrid=tmpGrid;
    skillBox->setLayout(skillGrid);

}
/*
 * called when the skill is updated
 * */

void CGenerator::updateSkill()
{
    for(int i=0;i<skillI.size();i++)
    {
        //We update the skill info
        skillData tmpData;
        QString oldName = skillD[i]->getInfo().name;
        qint32 realIndex=d->findSkillIndex(skillI[i].name->currentText());//Index in skillB

        tmpData.name=skillI[i].name->currentText();
        tmpData.speed=d->getSkillB()[realIndex]->getInfo().speed;//See data.h for the +10 / +20 offset
        tmpData.type=d->getSkillB()[realIndex]->getInfo().type;
        tmpData.special=skillI[i].special->currentIndex()+20;
        tmpData.level=skillI[i].level->value();
        tmpData.school=d->getSkillB()[realIndex]->getInfo().school;
        skillD[i]->setInfo(tmpData);
        skillI[i].speed->setCurrentIndex(skillD[i]->getInfo().speed);
        skillI[i].type->setCurrentIndex(skillD[i]->getInfo().type-10);
        if(oldName !=tmpData.name)//We changed the skill ; remove all perkList/Level from skillI, recreate a grid
        {
            qint32 tmpSize = skillI[i].perkList.size();
            for(int j=0;j< tmpSize;j++)
            {
                skillI[i].perkList.removeLast();
                skillI[i].perkLevel.removeLast();
                skillI[i].perkDesc.removeLast();
            }
            QGridLayout *tmpGrid;
            tmpGrid =new QGridLayout;
            tmpGrid->addWidget(new QLabel(tr("Nom")), 0,0);
            tmpGrid->addWidget(new QLabel(tr("Niveau")),0,1);
            tmpGrid->addWidget(new QLabel(tr("Spécial")),0,2);
            tmpGrid->addWidget(new QLabel(tr("Vitesse")),0,3);
            tmpGrid->addWidget(new QLabel(tr("Type")),0,4);
            tmpGrid->addWidget(skillI[i].name,1,0);
            tmpGrid->addWidget(skillI[i].level,1,1);
            tmpGrid->addWidget(skillI[i].special,1,2);
            tmpGrid->addWidget(skillI[i].speed,1,3);
            tmpGrid->addWidget(skillI[i].type,1,4);
            QLayoutItem *child;
            while ((child = skillInputLayout[i]->takeAt(0)) != 0)
            {
                delete child->widget();
                delete child;
            }
            delete skillInputLayout[i];
            skillInputLayout[i]=tmpGrid;
            skillInputBox[i]->setLayout(skillInputLayout[i]);

        }
        updatePerkNumber(i, realIndex);
        updatePerkData(i, realIndex);
        checkSkill(false);
    }
    updateCharData();

    calculateCost();
}

/*
 * Called in updateSkill, to change the perkNumber of one skill
 * */

void CGenerator::updatePerkNumber(qint32 curInd, qint32 realIndex)
{
    QList<CPerk *> tmpPerk=d->getUsablePerk(realIndex, skillD[curInd]->getInfo().level);
    QList<QStringList> perkL = newPerkL(curInd, tmpPerk);
    qint32 oldNumber = skillI[curInd].perkList.size();
    qint32 newNumber = perkL.size();
    if(skillD[curInd]->getInfo().name=="Forge technomagique")
    {
        return;
    }
    if( oldNumber < newNumber)//We have less perk than what we should have -> add perk
    {
        for(int i=oldNumber ; i<newNumber ; i++)
        {
            skillI[curInd].perkList << new QComboBox;
            skillI[curInd].perkLevel << new QSpinBox;
            skillI[curInd].perkDesc << new QLineEdit;
            perkNumberOkay << true;
            //Now we need to fill one by one every ComboBox or there is a bug (one QObiect is unique, this kind of shit)
            skillI[curInd].perkList[i]->addItems(perkL[i]);
            connect(skillI[curInd].perkList[i], SIGNAL(currentIndexChanged(int)), this, SLOT(updateSkill()));

            skillI[curInd].perkLevel[i]->setValue(d->getSkillB()[realIndex]->getPerkB()[0]->getParamInt("level"));
            skillI[curInd].perkLevel[i]->setMaximum(10);
            skillI[curInd].perkLevel[i]->setReadOnly(true);

            skillI[curInd].perkDesc[i]->setReadOnly(true);

            skillInputLayout[curInd]->addWidget(skillI[curInd].perkList[i],2+i,0);
            skillInputLayout[curInd]->addWidget(skillI[curInd].perkLevel[i],2+i,1);
            skillInputLayout[curInd]->addWidget(skillI[curInd].perkDesc[i],2+i,2,1,-1);
        }
    }
    else //We need to remove perk
    {
        QGridLayout* tmpGrid;
        tmpGrid = new QGridLayout;
        for(int i=0;i<skillInputLayout[curInd]->rowCount()-(oldNumber-newNumber);i++)
        {
            for(int j=0;j<skillInputLayout[curInd]->columnCount();j++)
            {
                QLayoutItem * const item = skillInputLayout[curInd]->itemAtPosition(i,j);
                if(dynamic_cast<QWidgetItem *>(item))
                {
                    if(j==2 && i>=2)//Little ugly but no other solution :(
                    {
                        tmpGrid->addWidget(item->widget(),i,j,1,-1);
                    }
                    else
                    {
                        tmpGrid->addWidget(item->widget(),i,j);
                    }

                }
            }
        }
        QLayoutItem *child;
        while ((child = skillInputLayout[curInd]->takeAt(0)) != 0)
        {
            delete child->widget();
            delete child;
        }
        delete skillInputLayout[curInd];
        skillInputLayout[curInd] = tmpGrid;
        skillInputBox[curInd]->setLayout(skillInputLayout[curInd]);
        for(int i=oldNumber;i>newNumber;i--)
        {
            skillI[curInd].perkLevel.removeLast();
            skillI[curInd].perkList.removeLast();
            skillI[curInd].perkDesc.removeLast();
        }
    }
}
/*
 * called inside updateSkill : put all the PerkData inside skillD
 * */
void CGenerator::updatePerkData(qint32 curInd, qint32 realIndex)
{
    skillD[curInd]->removeAllPerk();
    qint32 perkRealCost=0;
    for(int i=0;i<skillI[curInd].perkList.size();i++)
    {
        QString perkName = skillI[curInd].perkList[i]->currentText();
        CPerk* tmpPerk=d->getSkillB()[realIndex]->findPerk(perkName);
        if(perkName!="")
        {
            skillD[curInd]->addPerk(tmpPerk);
            skillI[curInd].perkLevel[i]->setValue(tmpPerk->getParamInt("level"));
            skillI[curInd].perkDesc[i]->setText(tmpPerk->getParamStr("desc"));
            skillI[curInd].perkDesc[i]->setToolTip(tmpPerk->getParamStr("desc"));
            skillI[curInd].perkDesc[i]->setCursorPosition(0);
        }
        skillI[curInd].perkList[i]->setToolTip("");
        skillI[curInd].perkList[i]->setStyleSheet("");
        perkNumberOkay[curInd]=true;

        perkRealCost = perkRealCost + tmpPerk->getParamInt("cost");
    }
    if((skillD[curInd]->isWeapon() || skillD[curInd]->isMagic())
            && perkRealCost > skillI[curInd].perkList.size())
    {
        qint32 emptyPerkNumber=0;
        for(int i=0;i<skillI[curInd].perkList.size();i++)
        {
            QString perkName = skillI[curInd].perkList[i]->currentText();
            if(perkName=="")
            {
                emptyPerkNumber++;
            }
            if(perkName !="" && skillD[curInd]->getPerkB()[i-emptyPerkNumber]->getParamInt("cost")>1)
            {
                QString toolTip = "Cette perk coûte plus d'un point ; retirez " + QString::number(perkRealCost - skillI[curInd].perkList.size()) ;
                toolTip = toolTip + " autre(s) perk (en choisissant la perk sans nom)";
                skillI[curInd].perkList[i]->setToolTip(toolTip);
                QString redStyle="QComboBox {border:1px solid red}";
                skillI[curInd].perkList[i]->setStyleSheet(redStyle);
                perkNumberOkay[curInd]=false;
            }
        }
    }
}
/*
 * */
void CGenerator::updateCharData()
{
    charD.name=charName->text();
    charD.race=raceI->currentText();
    charD.hp=caracI[0]->value();
    charD.strength=caracI[1]->value();
    charD.agility=caracI[2]->value();
    charD.reflexe=caracI[3]->value();
    charD.willpower=caracI[4]->value();
    charD.mana=caracI[5]->value();
    charB.hp=caracB[0]->value();
    charB.strength=caracB[1]->value();
    charB.agility=caracB[2]->value();
    charB.reflexe=caracB[3]->value();
    charB.willpower=caracB[4]->value();
    charB.mana=caracB[5]->value();
}
/*
 * used to initialize the QComboBox of perkList in a skillInput
 * */

QList<QStringList> CGenerator::newPerkL(qint32 curInd, QList<CPerk *> perkBaseList)
{
    QList<QStringList> perkL;//Will be used to generate the QComboBox later
    qint32 perkN = (skillD[curInd]->getInfo().level + 1) / 2;
    perkN = (skillD[curInd]->getInfo().level == 9) ? perkN -1 : perkN;//Cuz there is a 3 level gap between 7 and 10, 9 would have too many perk

    if(skillD[curInd]->isWeapon() || skillD[curInd]->isMagic())//Weapon- magic
    {
        perkN = skillD[curInd]->getInfo().level;
        perkN = (skillD[curInd]->getInfo().special==22) ? perkN +1 : perkN;//Gift -> +1 point
        for(int i=0;i<perkN;i++)
        {
            perkL << QStringList();
            for(int j=0;j<perkBaseList.size();j++)
            {
                if( i+1 >=perkBaseList[j]->getParamInt("level"))
                {
                perkL[i] << perkBaseList[j]->getParamStr("name");
                }
            }
            perkL[i] << "";
        }
    }
    else if(skillD[curInd]->getInfo().name=="Technologie")
    {
        perkN++;//We have the perk at level 0
        for(int i=0;i<perkN;i++)
        {
            perkL << QStringList();
            for(int j=0;j<perkBaseList.size();j++)
            {
                //Level 0 & level 10 special case
                if((perkBaseList[j]->getParamInt("level")==0 && i==0) || (perkBaseList[j]->getParamInt("level")==10 && i==perkN-1))
                {
                    perkL[i] << perkBaseList[j]->getParamStr("name");
                }
                //Others level
                if(2*i-1==perkBaseList[j]->getParamInt("level") && i!=0)
                {
                perkL[i] << perkBaseList[j]->getParamStr("name");
                }
            }
        }
    }
    else//All other skills
    {
        for(int i=0;i<perkN;i++)
        {
            perkL << QStringList();
            for(int j=0;j<perkBaseList.size();j++)
            {
                if(2*i+1==perkBaseList[j]->getParamInt("level") || (perkBaseList[j]->getParamInt("level")==10 && i==4))
                {
                perkL[i] << perkBaseList[j]->getParamStr("name");
                }
            }
        }
    }
    return perkL;

}

void CGenerator::updateCarac()
{
    //Put the base value for a race into caracB[i]
    selectedRaceIndex=raceI->currentIndex();
    selectedRace=raceI->currentText();
    for (int i=0 ; i < d->getNumCarac() ; ++i)
    {
        qDebug() << selectedRaceIndex << d->getRaceB().size();
        caracB[i]->setValue(d->getRaceB()[selectedRaceIndex]->getCarac(i));
        caracI[i]->setValue(caracB[i]->value());
    }
    updateSkill();
}

void CGenerator::updateFont()
{
    bool ok;
    usedFont = QFontDialog::getFont(&ok, usedFont, this);
    fontName->setText(usedFont.family());
}

void CGenerator::calculateCost()
{
    qint32 cost=advantageValue->value();
    //Race point
    cost+=d->getRaceB()[selectedRaceIndex]->getCarac("cost");
    //Carac point
    if((charD.hp-charB.hp)%2==0)
    {
        cost+=d->getCaracCost()[0]*(charD.hp -charB.hp)/2;
    }
    else
    {
        cost+=d->getCaracCost()[0]*(charD.hp + 1 -charB.hp)/2;
    }
    cost+=d->getCaracCost()[1]*(charD.strength -charB.strength);
    cost+=d->getCaracCost()[2]*(charD.agility -charB.agility);
    cost+=d->getCaracCost()[3]*(charD.reflexe -charB.reflexe);
    cost+=d->getCaracCost()[4]*(charD.willpower -charB.willpower);
    if((charD.mana-charB.mana)%3==0)
    {
        cost+=d->getCaracCost()[5]*(charD.mana -charB.mana)/3;
    }
    else
    {
        cost+=d->getCaracCost()[5]*(3-(charD.mana -charB.mana)%3 + charD.mana -charB.mana)/3;
    }
    QList<qint32> specialCost;

    if(selectedRace=="Demi Elfe" || selectedRace=="Pyrde")
    {
        specialCost << 0 << 60 << 160;
    }
    else
    {
        specialCost=d->getSpecialCost();
    }
    //Skill point
    qint32 skillCost=0;
    for(int i=0;i<skillD.size();i++)
    {
        if((selectedRace=="Nain" || selectedRace=="Gnome")//Techno middle for dwarf/gnome
           && skillD[i]->getInfo().name=="Technologie")
        {
            skillData tmpInfo;
            tmpInfo=skillD[i]->getInfo();
            tmpInfo.speed=1;
            skillD[i]->setInfo(tmpInfo);
        }
        else if(selectedRace=="Vaerilien" && skillD[i]->getInfo().name=="Portail")
        {
            skillData tmpInfo;
            tmpInfo=skillD[i]->getInfo();
            tmpInfo.speed=1;
            skillD[i]->setInfo(tmpInfo);
        }
        else if(selectedRace=="Troll ancien" && skillD[i]->getInfo().name=="Psychique")
        {
            skillData tmpInfo;
            tmpInfo=skillD[i]->getInfo();
            tmpInfo.speed=1;
            skillD[i]->setInfo(tmpInfo);
        }
        skillCost+= d->getSkillCost()[skillD[i]->getInfo().speed][skillD[i]->getInfo().level];
        skillCost+=specialCost[skillD[i]->getInfo().special-20];
    }
    qint32 giftedPoint=raceGiftPoint(false);

    if(skillCost-giftedPoint<0)
    {
        giftedPoint=skillCost;
    }

    cost+=skillCost-giftedPoint;
    charD.point=cost;
    pointValue->setValue(cost);
}
/*
 * */
qint32 CGenerator::raceGiftPoint(bool verbose)
{
    /*
    QString tmpRaceGift=d->getRaceGift()[selectedRaceIndex];
    qint32 giftNumber=tmpRaceGift.count("+")+1;
    qint32 giftedPoint=0;
    qint32 giftedIndex=-1;
    for(int i=0;i<giftNumber;i++)
    {
        QString storage="";
        qint32 pointStorage=0;
        qint32 indexStorage=-1;
        if(giftNumber>1)
        {
            storage=tmpRaceGift;
            tmpRaceGift=tmpRaceGift.left(tmpRaceGift.indexOf("+"));
            storage.remove(0, storage.indexOf("+")+1);
            pointStorage=giftedPoint;
            indexStorage=giftedIndex;
            giftedPoint=0;
        }
        qint32 separator=tmpRaceGift.indexOf("-");
        QString tmpSkillType=tmpRaceGift.left(separator);
        qint32 skillLevel = tmpRaceGift.remove(0,tmpRaceGift.indexOf("-")+1).toInt();
        bool reverse=false;
        if(tmpSkillType.at(0)=='!')
        {
            reverse=true;
            tmpSkillType=tmpSkillType.remove(0,1);
        }
        qint32 skillType=tmpSkillType.toInt();

        if(!tmpSkillType.at(0).isDigit())//We are looking for a skill name
        {
            for(int j=0;j<skillD.size();j++)
            {
                qint32 realIndex=d->findSkillIndex(skillD[j]->getInfo().name);
                if(tmpSkillType==skillD[j]->getInfo().name  && indexStorage!=realIndex)
                {
                    giftedPoint = d->getSkillCost()[skillD[j]->getInfo().speed][skillLevel];
                    giftedIndex=j;
                }
            }
            if(giftedPoint==0 && verbose)
            {
                QMessageBox::information(this, "Compétence gratuite non prise","Vous avez la compétence "
                + tmpSkillType + " au niveau " + QString::number(skillLevel)
                +" ,offerte grâce à votre race ("+ selectedRace +") que vous n'avez pas prise.");
            }

        }
        else if(skillType==10 && reverse)
        {
            for(int j=0;j<skillD.size();j++)
            {
                qint32 realIndex=d->findSkillIndex(skillD[j]->getInfo().name);
                if(giftedPoint < d->getSkillCost()[skillD[j]->getInfo().speed][skillLevel] && indexStorage!=realIndex
                   && !skillD[j]->isWeapon())
                {
                    giftedPoint = d->getSkillCost()[skillD[j]->getInfo().speed][skillLevel];
                    giftedIndex=d->findSkillIndex(skillD[j]->getInfo().name);

                }
            }
        }
        else if(skillType==11 && reverse)
        {
            for(int j=0;j<skillD.size();j++)
            {
                qint32 realIndex=d->findSkillIndex(skillD[j]->getInfo().name);
                if(giftedPoint < d->getSkillCost()[skillD[j]->getInfo().speed][skillLevel] && indexStorage!=realIndex
                   && !skillD[j]->isMagic())
                {
                    giftedPoint = d->getSkillCost()[skillD[j]->getInfo().speed][skillLevel];
                    giftedIndex=j;

                }
            }
        }
        else if(skillType==9)
        {
            for(int j=0;j<skillD.size();j++)
            {
                qint32 realIndex=d->findSkillIndex(skillD[j]->getInfo().name);
                if(giftedPoint < d->getSkillCost()[skillD[j]->getInfo().speed][skillLevel] && indexStorage!=realIndex)
                {
                    giftedPoint = d->getSkillCost()[skillD[j]->getInfo().speed][skillLevel];
                    giftedIndex=j;

                }
            }

        }
        else if(skillType==10)
        {
            for(int j=0;j<skillD.size();j++)
            {
                qint32 realIndex=d->findSkillIndex(skillD[j]->getInfo().name);
                if(giftedPoint < d->getSkillCost()[skillD[j]->getInfo().speed][skillLevel] && indexStorage!=realIndex
                   && skillD[j]->isWeapon())
                {
                    giftedPoint = d->getSkillCost()[skillD[j]->getInfo().speed][skillLevel];
                    giftedIndex=d->findSkillIndex(skillD[j]->getInfo().name);

                }
            }
        }
        else if(skillType==11)
        {
            for(int j=0;j<skillD.size();j++)
            {
                qint32 realIndex=d->findSkillIndex(skillD[j]->getInfo().name);
                if(giftedPoint < d->getSkillCost()[skillD[j]->getInfo().speed][skillLevel] && indexStorage!=realIndex
                   && (skillD[j]->isMagic() || skillD[j]->getInfo().name=="Art de la magie"))
                {
                    giftedPoint = d->getSkillCost()[skillD[j]->getInfo().speed][skillLevel];
                    giftedIndex=d->findSkillIndex(skillD[j]->getInfo().name);

                }
            }
        }
        else if(skillType==12)
        {
            for(int j=0;j<skillD.size();j++)
            {
                qint32 realIndex=d->findSkillIndex(skillD[j]->getInfo().name);
                if(giftedPoint < d->getSkillCost()[skillD[j]->getInfo().speed][skillLevel] && indexStorage!=realIndex
                   && skillD[j]->isMisc())
                {
                    giftedPoint = d->getSkillCost()[skillD[j]->getInfo().speed][skillLevel];
                    giftedIndex=d->findSkillIndex(skillD[j]->getInfo().name);

                }
            }
        }
        if(giftedPoint==0 && verbose)
        {
            QString tmpDesc="Vous avez le droit à une compétence ";
            tmpDesc= reverse ? tmpDesc + "non " : tmpDesc;
            tmpDesc= skillType==10 ? tmpDesc + "martiale " : tmpDesc;
            tmpDesc = skillType==11 ? tmpDesc + "magique " : tmpDesc;
            tmpDesc = skillType==9 ? tmpDesc + "au choix " : tmpDesc;
            tmpDesc+="au niveau " + QString::number(skillLevel) + " offerte grâce à votre race (";
            tmpDesc+=selectedRace +").";
            QMessageBox::information(this, "Compétence offerte non prise",
                                     tmpDesc);
        }

        tmpRaceGift=storage;
        giftedPoint = giftedPoint + pointStorage;
    }//For i<giftNumber
    return giftedPoint;
    */
    return 0;
}

void CGenerator::generateSheet()
{
    updateSkill();
    bool strictCheck=d->getSettings()->value("strictCheck").value<bool>();
    if( (checkSkill(true) && strictCheck) || !strictCheck)
    {
        raceGiftPoint(true);
        writer = new COdtWriter(charD,skillD, usedFont, d, religionPowerD);
    }
    else
    {
        QMessageBox::warning(this, "Validation ratée",
"La validation a échoué et nous sommes en mode nazi, changez le kustom.ini");
    }

}

void CGenerator::religionEffect()
{
    /*
    qint32 cost=0;
    QList<qint32> specialChosen;
    religionPowerD.clear();
    for(int i=0;i<d->getReligionB().size();i++)
    {
        for(int j=0;j<d->getReligionB()[i]->size();j++)
        {
            if(religionPowerChoice[i][j])
            {
                cost+=d->getReligionB()[i][j]->cost;
                religionPowerD << d->getReligionB()[i][j];
                if(d->getReligionB()[i][j].special != 0)
                {
                    specialChosen << d->getReligionB()[i][j].special;
                }
            }
        }
    }
    advantageValue->setValue(cost);
    /*Here we deal with all the special things religion add (not speaking about priest & paedophilia)
     * Like additionnal stat, those kind of thing (need to find a good way to display them btw)
     * 11
     *
    qint32 religionHp=0;
    qint32 religionStrength=0;
    if(specialChosen.indexOf(11) != -1)//Prière de complexion
    {
        religionHp+=10;
    }
    if(specialChosen.indexOf(12) != -1)//Membru de chyrma
    {
        religionHp+=10;
        religionStrength+=4;
    }
    if(specialChosen.indexOf(13) != -1)//Élu des Dieux
    {
        religionHp+=25;
    }
    if(specialChosen.indexOf(84) != -1)//Force de la nature
    {
        religionHp+=25;
        religionStrength+=20;
    }
    charD.religionHp=religionHp;
    charD.religionStrength=religionStrength;
    */
}

/*
 * */
bool CGenerator::checkSkill(bool verbose)
{
    bool output=true;
    //Cost of perk OK
    for(int i=0;i<perkNumberOkay.size();i++)
    {
        if(!perkNumberOkay[i])
        {
            if(verbose)
            {
                QMessageBox::warning(this, "Nombre de perk incohérent",
                "Vous avez au moins une perk en trop dans la compétence "+skillD[i]->getInfo().name);
            }
            output=false;
        }
    }
    //All perk different
    QStringList tmpSkillL;

    for(int i=0;i<skillD.size();i++)
    {
        QStringList tmpPerkL;
        tmpSkillL<<skillD[i]->getInfo().name;
        for(int j=0;j<skillD[i]->getPerkB().size();j++)
        {
            tmpPerkL << skillD[i]->getPerkB()[j]->getParamStr("name");
        }
        if(tmpPerkL.removeDuplicates()!=0 && !skillD[i]->isMisc())//Misc skill can't have 2 time the same skill
        {
            if(verbose)
            {
                QMessageBox::warning(this, "Perk(s) schizophrène(s)",
                "Vous avez pris au moins deux fois la même perk dans la compétence " +skillD[i]->getInfo().name);
            }
            output=false;
        }
    }
    //All skill different
    if(tmpSkillL.removeDuplicates()!=0)
    {
        if(verbose)
        {
            QMessageBox::warning(this, "Compétence(s) schizophrène(s)",
                                 "Vous avez au moins une compétence en double (ou plus)");
        }
        output=false;
    }
    //TODO : refaire
    /*
    //Other perk/skill needed
    for(int i=0;i<skillD.size();i++)
    {
        for(int j=0;j<skillD[i]->getPerkB().size();j++)
        {
            bool isSkill=false;
            QString needed="";
            qint32 neededSeparator=0;
            if(!skillD[i]->getPerkB()[j].levelNeeded.isEmpty())
            {
                foreach(QChar c, skillD[i]->getPerkB()[j].levelNeeded)
                {
                    if(c.isDigit())//It's another skill
                    {
                        isSkill=true;
                        needed=skillD[i]->getPerkB()[j].levelNeeded;
                        neededSeparator=needed.indexOf(c);
                        break;
                    }
                }
                if(isSkill)//We need another skill
                {
                    QString neededLevel=needed.right(2);
                    if(neededLevel.indexOf("")!=1)
                    {
                        neededLevel=neededLevel.right(1);
                    }
                    needed=needed.left(neededSeparator);
                    needed.remove(needed.size()-1,1);
                    QString needed1, needed2;
                    needed1=needed2="";
                    if(needed.indexOf("ou")!=-1)//We can have differents skills
                    {
                        needed1=needed.left(needed.indexOf("ou")).simplified();
                        needed2=needed.remove(0,needed.indexOf("ou")+2).simplified();//+2 for the "ou"
                    }
                    for(int k=0;k<skillD.size();k++)
                    {
                        if(skillD[k]->getInfo().level>=neededLevel.toInt() &&
                     (skillD[k]->getInfo().name==needed || skillD[k]->getInfo().name==needed1 || skillD[k]->getInfo().name==needed2))
                        {
                            break;
                        }
                        if(needed=="Arme" && skillD[k]->getInfo().type==10
                        && skillD[k]->getInfo().name!=skillD[i]->getInfo().name)//Could've been the same if but lisiblity
                        {
                            qDebug() << "test";
                            break;
                        }
                        if(k+1==skillD.size())
                        {
                            QString tmpWarning = "il vous manque la compétence ";
                            tmpWarning = needed1.isEmpty() ? tmpWarning + needed : tmpWarning + needed1 + " ou " + needed2;
                            tmpWarning = tmpWarning + " au niveau " + neededLevel;
                            tmpWarning = tmpWarning + " pour la perk " +skillD[i]->getPerkB()[j].name ;
                            tmpWarning = tmpWarning + " de la compétence " + skillD[i]->getInfo().name;
                            if(verbose)
                            {
                                QMessageBox::warning(this, "Compétence manquante", tmpWarning);
                            }
                            else
                            {
                                //We find the perk number inside skillI (may differ cause of empty perk not added in skillD)
                                for(int l=0;l<skillI[i].perkList.size();l++)
                                {
                                    if(skillD[i]->getPerkB()[j].name==skillI[i].perkList[l]->currentText())
                                    {
                                        QString greenStyle="QComboBox {border:1px solid green}";
                                        skillI[i].perkList[l]->setStyleSheet(greenStyle);
                                        skillI[i].perkList[l]->setToolTip(tmpWarning);
                                        break;
                                    }
                                }
                            }
                            output=false;
                        }
                    }

                }//if(isSkill)
                else//It's a perk we need
                {
                    needed = skillD[i]->getPerkB()[j].levelNeeded;
                    for(int k=0;k<skillD[i]->getPerkB().size();k++)
                    {
                        if(skillD[i]->getPerkB()[k].name==needed)
                        {
                            break;
                        }

                        if(k+1==skillD[i]->getPerkB().size())//Nothing worked -> prerequis not ok
                        {
                            QString tmpWarning = "il vous manque la perk ";
                            tmpWarning = tmpWarning + needed + " qui est nécessaire pour la perk ";
                            tmpWarning = tmpWarning + skillD[i]->getPerkB()[j].name;
                            tmpWarning = tmpWarning + " dans la compétence " + skillD[i]->getInfo().name;
                            if(verbose)
                            {
                                QMessageBox::warning(this, "Perk manquante", tmpWarning);
                            }
                            else
                            {
                                //We find the perk number inside skillI (may differ cause of empty perk not added in skillD)
                                for(int l=0;l<skillI[i].perkList.size();l++)
                                {
                                    if(skillD[i]->getPerkB()[j].name==skillI[i].perkList[l]->currentText())
                                    {
                                        QString greenStyle="QComboBox {border:1px solid green}";
                                        skillI[i].perkList[l]->setStyleSheet(greenStyle);
                                        skillI[i].perkList[l]->setToolTip(tmpWarning);
                                        break;
                                    }
                                }
                            }
                            output=false;
                        }
                    }
                }

            }
        }
    }
    */
    return output;

}

void CGenerator::resetSheet()
{
    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Remise à zéro de la fiche", "Êtes-vous sûr de vouloir remettre à zéro ?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes)
      {
          //Reset
          /*Nom -> VIDE
           * race -> humain (indice 0)
           * pts supp -> 0
           * stats -> reset via race humain
           * religion -> mettre à false tout
           * */
          charName->setText("");
          advantageValue->setValue(0);
          raceI->setCurrentIndex(0);
          updateCarac();
          qint32 skillNumber=skillD.size();
          for(int i=skillNumber-1;i>=0;i--)
          {
              removeSkillAtInd(i);
              updateSkill();
          }
          for(int i=0;i<religionPowerChoice.size();i++)
          {
              for(int j=0;j<religionPowerChoice[i].size();j++)
              {
                  religionPowerChoice[i][j]=false;
              }
          }
          religionEffect();
      }
}

void CGenerator::addSkillDialog()
{
    _dialogNewSkill = new CDialogAddSkill(this);
}


void CGenerator::removeSkillDialog()
{
    _dialogRemoveSkill = new CDialogRemoveSkill(this);
}

void CGenerator::religionDialog()
{
    _dialogReligion = new CDialogReligion(this);
    connect(_dialogReligion, SIGNAL(destroyed(QObject*)), this, SLOT(religionEffect()));

}

void CGenerator::infoSkillDialog()
{
    _dialogInfoSkill = new CDialogInfoSkill(this);
}
/*
 * */
QList<CSkill *> CGenerator::getSkillD() const
{
    return skillD;
}
QList<QList<bool > > CGenerator::getReligionPowerChoice() const
{
    return religionPowerChoice;
}

void CGenerator::setReligionPowerChoice(int i, int j, bool value)
{
    if(i<religionPowerChoice.size() && j<religionPowerChoice[i].size())
    {
        religionPowerChoice[i][j]=value;
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
