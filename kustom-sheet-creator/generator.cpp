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
#include "odt_writer.h"
#include "dialog.h"
CGenerator::CGenerator(CData *data)
{

    d=data;
    _c=new CCharacter(d);
    createPersonalBox();
    createCaracBox();
    createSkillBox();
    createGeneratorLayout();
    setLayout(_generatorLayout);
    onRaceChanged();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkChange()));
    timer->start(10);

}
void CGenerator::createPersonalBox()
{
    _personalGrid = new QGridLayout;
    _personalBox = new QGroupBox("Infos générales");
    _charName = new QLineEdit;
    _raceI = new QComboBox;
    _pointValue = new QSpinBox;
    _pointValue->setMaximum(100000);
    _pointValue->setReadOnly(true);
    _advantageValue = new QSpinBox;
    _advantageValue->setMaximum(100000);
    _religionValue = new QSpinBox;
    _religionValue->setMaximum(100000);
    _religionValue->setReadOnly(true);
    _usedFont= QFont(d->getSettings()->value("baseFont").value<QString>(), 12);
    _fontNameEdit = new QLineEdit;
    _fontNameEdit->setReadOnly(true);
    if(!_usedFont.exactMatch())
    {
        _usedFont.setFamily(d->getSettings()->value("fallbackFont").value<QString>());
    }
    _fontNameEdit->setText(_usedFont.family());

    _buttonFont = new QPushButton("Changer la police");
    for (int i=0; i<d->getRaceB().size();++i)
    {
        _raceI->addItem(d->getRaceB()[i]->getName());
    }
    connect(_raceI, SIGNAL(currentIndexChanged(int)), this, SLOT(onRaceChanged()));
    connect(_charName, SIGNAL(textChanged(QString)), this ,SLOT(onNameChanged()));
    connect(_advantageValue, SIGNAL(valueChanged(int)), this, SLOT(updatePointValue()));
    connect(_buttonFont, SIGNAL(clicked()), this, SLOT(onFontChanged()));
    _personalGrid->addWidget(new QLabel(tr("Nom :")),0,0);
    _personalGrid->addWidget(new QLabel(tr("Votre race")),1,0);
    _personalGrid->addWidget(new QLabel(tr("Points supplémentaires")),2,0);
    _personalGrid->itemAtPosition(2,0)->widget()->setToolTip("Points non inclus dans le calculateur (avantages principalement");
    _personalGrid->addWidget(new QLabel(tr("Points de religion")),3,0);
    _personalGrid->addWidget(new QLabel(tr("Points Kustom")),4,0);
    _personalGrid->itemAtPosition(4,0)->widget()->setToolTip("Valeur calculée en K pour votre personnage");
    _personalGrid->addWidget(new QLabel(tr("Police ")),5,0);
    _personalGrid->addWidget(_charName,0,1);
    _personalGrid->addWidget(_raceI,1,1);
    _personalGrid->addWidget(_advantageValue,2,1);
    _personalGrid->addWidget(_religionValue,3,1);
    _personalGrid->addWidget(_pointValue,4,1);
    _personalGrid->addWidget(_fontNameEdit,5,1);
    _personalGrid->addWidget(_buttonFont, 6,0, 1, -1);
    _personalBox->setLayout(_personalGrid);
}

void CGenerator::createCaracBox()
{
    _attributeGrid = new QGridLayout;
    _attributeBox = new QGroupBox("Caractéristiques");
    _attributeGrid->addWidget(new QLabel(tr("Personnage")),0, 1);
    _attributeGrid->addWidget(new QLabel(tr("Base (de la race)")),0,2);
    for (int i=0; i < d->getNumCarac() ; ++i)
    {
        QSpinBox *tmpSpinI, *tmpSpinB;
        tmpSpinI = new QSpinBox;
        tmpSpinB = new QSpinBox;
        tmpSpinI->setMaximum(1000);
        tmpSpinB->setMaximum(1000);
        _attributeI.append(tmpSpinI);
        _attributeB.append(tmpSpinB);
        _attributeB[i]->setReadOnly(true);
        _attributeGrid->addWidget(new QLabel(d->getCaracName()[i]), i + 1, 0);
        _attributeGrid->addWidget(_attributeI[i], i + 1, 1);
        _attributeGrid->addWidget(_attributeB[i], i + 1, 2);
        connect(_attributeI[i], SIGNAL(valueChanged(int)), this, SLOT(onAttributeChanged()));
    }
    _attributeBox->setLayout(_attributeGrid);
}

void CGenerator::createSkillBox()
{
    _skillGrid = new QGridLayout;
    _skillBox = new QGroupBox("Compétences");
    _skillBox->setLayout(_skillGrid);
}

void CGenerator::createGeneratorLayout()
{
    _generatorLayout = new QVBoxLayout;
    _topLayout = new QHBoxLayout;
    _topBox = new QGroupBox("");
    _topLayout->addWidget(_personalBox,0,Qt::AlignTop);
    _topLayout->addWidget(_attributeBox,0,Qt::AlignTop);
    _topBox->setLayout(_topLayout);

    _buttonLayout = new QHBoxLayout;
    _buttonBox = new QGroupBox("");

    _buttonAddSkill = new QPushButton("Ajouter une compétence");
    _buttonRemoveSkill = new QPushButton("Supprimer une compétence");
    _buttonReligion = new QPushButton("Pouvoirs de religions");
    _buttonReset = new QPushButton("Remise à zéro");
    _buttonWriteSheet = new QPushButton("Générer la fiche");
    connect(_buttonAddSkill, SIGNAL(clicked()), this, SLOT(addSkillDialog()));
    connect(_buttonRemoveSkill, SIGNAL(clicked()), this, SLOT(removeSkillDialog()));
    connect(_buttonReligion, SIGNAL(clicked(bool)), this, SLOT(religionDialog()));
    connect(_buttonReset, SIGNAL(clicked(bool)), this, SLOT(resetSheet()));
    connect(_buttonWriteSheet, SIGNAL(clicked()), this, SLOT(writeSheet()));
    _buttonLayout->addWidget(_buttonAddSkill);
    _buttonLayout->addWidget(_buttonRemoveSkill);
    _buttonLayout->addWidget(_buttonReligion);
    _buttonLayout->addWidget(_buttonReset);
    _buttonLayout->addWidget(_buttonWriteSheet);
    //For debug only, i should add something or what
    _buttonDebug= new QPushButton("Infos debug");
    connect(_buttonDebug, SIGNAL(clicked()), this, SLOT(displayCharacter()));
    _buttonLayout->addWidget(_buttonDebug);
    _buttonBox->setLayout(_buttonLayout);

    _skillScrollArea = new CVerticalScrollArea(_skillBox);
    _generatorLayout->addWidget(_topBox);
    _generatorLayout->addWidget(_skillScrollArea);
    _generatorLayout->addWidget(_buttonBox);
}

void CGenerator::onAttributeChanged()
{
        _c->setAttribute("health", _attributeI[0]->value());
        _c->setAttribute("strength", _attributeI[1]->value());
        _c->setAttribute("agility", _attributeI[2]->value());
        _c->setAttribute("reflexe", _attributeI[3]->value());
        _c->setAttribute("willpower", _attributeI[4]->value());
        _c->setAttribute("mana", _attributeI[5]->value());
        updatePointValue();
}

void CGenerator::onRaceChanged()
{
    _c->updateRace(d->getFullId("race", _raceI->currentIndex()) );
    updateSkillGUI();
    _attributeB[0]->setValue( d->getRaceById(_c->getRaceId())->getAttribute("health"));
    _attributeB[1]->setValue( d->getRaceById(_c->getRaceId())->getAttribute("strength"));
    _attributeB[2]->setValue( d->getRaceById(_c->getRaceId())->getAttribute("agility"));
    _attributeB[3]->setValue( d->getRaceById(_c->getRaceId())->getAttribute("reflexe"));
    _attributeB[4]->setValue( d->getRaceById(_c->getRaceId())->getAttribute("willpower"));
    _attributeB[5]->setValue( d->getRaceById(_c->getRaceId())->getAttribute("mana"));

    _attributeI[0]->setValue( _attributeB[0]->value());
    _attributeI[1]->setValue( _attributeB[1]->value());
    _attributeI[2]->setValue( _attributeB[2]->value());
    _attributeI[3]->setValue( _attributeB[3]->value());
    _attributeI[4]->setValue( _attributeB[4]->value());
    _attributeI[5]->setValue( _attributeB[5]->value());
}

void CGenerator::onNameChanged()
{
    _c->setName(_charName->text());
}
void CGenerator::onFontChanged()
{
    bool ok;
    _usedFont = QFontDialog::getFont(&ok, _usedFont, this);
    _fontNameEdit->setText(_usedFont.family());
}

void CGenerator::updatePointValue()
{
    _c->setAdvantagePoints(_advantageValue->value());
    _religionValue->setValue(_c->getReligionPoints());
    _c->updatePointsTotal();
    _pointValue->setValue(_c->getPointsTotal());
}

void CGenerator::addSkillDialog()
{
    _dialogAddSkill = new CDialogAddSkill(this, _c->getSkill().size());
}

void CGenerator::religionDialog()
{
    _dialogReligion = new CDialogReligion(this, _c->getReligionPowerList());
}

void CGenerator::removeSkillDialog()
{
    QStringList skillName;
    for(int i=0;i<_c->getSkill().size();i++)
    {
        QString name = _c->getSkill()[i]->getParam("name") + " - " + _c->getSkill()[i]->getParam("level");
        if(_c->getSkill()[i]->getParam("special")=="t")
        {
            name += " (Talent)";
        }
        else if(_c->getSkill()[i]->getParam("special")=="g")
        {
            name += " (Don)";
        }
        skillName << name;
    }
    _dialogRemoveSkill = new CDialogRemoveSkill(this, skillName);
}

void CGenerator::writeSheet()
{
    //bool strictCheck=d->getSettings()->value("strictCheck").value<bool>();
    _c->updatePointsTotal(true);
    checkSkillPrerequisite(true);
    for(int i=0;i<_c->getSkill().size();i++)
    {
        CUSkill *skill = _c->getSkill()[i];
        if(skill->multipleSchool() && skill->getParam("special")=="t")
        {
            bool ok;
            QString school = QInputDialog::getItem(this, "Choix de l'école pour le talent","École de "+skill->getParam("name"),
                                                   skill->getSchools(),0, false, &ok);
            skill->setSchool(school);
        }
    }
    //The list of schools is a QStringList joined with "," so we need2split


    //tmpName = skill->getParam("special")=="t" ? tmpName + " (talent : "+ school +")" : tmpName;
    _writer = new COdtWriter(_c, _usedFont, d);
}

void CGenerator::checkSkillPrerequisite(bool verbose)
{
    QStringList userSkillName;
    for(int i=0;i<_c->getSkill().size();i++)
    {
        CUSkill *skill = _c->getSkill()[i];
        userSkillName << skill->getParam("name");//Used later to check for any duplicates

        if(verbose && skill->excessPerkNumber()>0)
        {
            QMessageBox::warning(this, "Nombre de perk incohérent",
            "Vous avez au moins une perk en trop dans la compétence "+skill->getParam("name"));
        }
        for(int j=0;j<skill->getPerkB().size();j++)
        {
            CPerk *perk = skill->getPerkB()[j];
            if(perk->getParamInt("cost") > 1 && skill->excessPerkNumber()>0)
            {
                QString toolTip = "Cette perk coûte plus d'un point ; retirez "
                        + QString::number(skill->excessPerkNumber()) ;
                toolTip = toolTip + " autre(s) perk (en choisissant la perk sans nom)";
                _skillI[i]->perkList[j]->setToolTip(toolTip);
                QString redStyle="QComboBox {border:1px solid red}";
                _skillI[i]->perkList[j]->setStyleSheet(redStyle);
            }
            else
            {
                QComboBox *tmpStyle = new QComboBox;
                _skillI[i]->perkList[j]->setStyleSheet(tmpStyle->styleSheet());
                _skillI[i]->perkList[j]->setToolTip("");
            }
        }

        QStringList perkName;
        //Prerequisite
        for(int j=0;j<skill->getPerkB().size();j++)//We check for perk prerequisite
        {
            CPerk *perk = skill->getPerkB()[j];
            QStringList needed = perk->getPrerequisite().keys();
            if(perk->getParamStr("id") !="-1")
            {

            perkName << perk->getParamStr("name");
            }
            if(perk->IsPrerequisitePerk())
            {
                bool ok=false;
                for(int k=0;k<skill->getPerkB().size();k++)
                {
                    if(needed[0] ==skill->getPerkB()[k]->getParamStr("id"))
                    {
                        ok=true;
                        break;
                    }
                }
                if(!ok)
                {
                    QString perkNeededName = d->getPerkById(needed[0])->getParamStr("name");
                    QString tmpWarning = "il vous manque la perk ";
                    tmpWarning = tmpWarning + perkNeededName + " qui est nécessaire pour la perk ";
                    tmpWarning = tmpWarning + perk->getParamStr("name");
                    tmpWarning = tmpWarning + " dans la compétence " + skill->getParam("name");
                    if(verbose)
                    {
                        QMessageBox::warning(this, "Perk manquante", tmpWarning);
                    }
                    QString greenStyle="QComboBox {border:1px solid green}";
                    _skillI[i]->perkList[j]->setStyleSheet(greenStyle);
                    _skillI[i]->perkList[j]->setToolTip(tmpWarning);
                }
                else
                {
                    QComboBox *tmpStyle = new QComboBox;
                    _skillI[i]->perkList[j]->setStyleSheet(tmpStyle->styleSheet());
                    _skillI[i]->perkList[j]->setToolTip("");
                }
            }
            else if(perk->havePrerequisite())//We need a skill
            {
                bool ok=false;
                for(int neededInd=0;neededInd<needed.size();neededInd++)
                {
                    for(int k=0;k<_c->getSkill().size();k++)
                    {
                        CUSkill *tmpSkill = _c->getSkill()[k];
                        if(needed[neededInd] == tmpSkill->getParam("id")
                                && tmpSkill->getParam("level").toInt() >= perk->getPrerequisite().value(needed[neededInd]))
                        {
                            ok=true;
                        }
                    }
                }
                if(!ok)
                {
                    QString tmpWarning = "il vous manque la compétence ";
                    if(needed.size() >1)
                    {
                        tmpWarning += d->getSkillById(needed[0])->getParam("name") + " ou " +
                                d->getSkillById(needed[1])->getParam("name");
                    }
                    else
                    {
                        tmpWarning += d->getSkillById(needed[0])->getParam("name");
                    }
                    tmpWarning = tmpWarning + " au niveau " + QString::number(perk->getPrerequisite().value(needed[0]));
                    tmpWarning = tmpWarning + " pour la perk " + perk->getParamStr("name");
                    tmpWarning = tmpWarning + " de la compétence " + skill->getParam("name");
                    if(verbose)
                    {
                        QMessageBox::warning(this, "Compétence manquante", tmpWarning);
                    }
                    else
                    {

                        QString greenStyle="QComboBox {border:1px solid green}";
                        _skillI[i]->perkList[j]->setStyleSheet(greenStyle);
                        _skillI[i]->perkList[j]->setToolTip(tmpWarning);
                    }
                }
                else
                {
                    QComboBox *tmpStyle = new QComboBox;
                    _skillI[i]->perkList[j]->setStyleSheet(tmpStyle->styleSheet());
                    _skillI[i]->perkList[j]->setToolTip("");
                }
            }
        }
        //Duplicate perk
        if(perkName.removeDuplicates()!=0 && !skill->isMisc())
        {
            if(verbose)
            {
                QMessageBox::warning(this, "Perk(s) schizophrène(s)",
                "Vous avez pris au moins deux fois la même perk dans la compétence " +skill->getParam("name"));
            }
        }
    }
}
/*
 * */

void CGenerator::addSkillFromDialog()
{
    //First we add the data inside _c
    CSkillInput *input = _dialogAddSkill->tmpInput;
    QString id=input->name->itemData(input->name->currentIndex()).toString();
    _c->addSkill(id);

    qint32 skillIndex = _c->getSkill().size()-1;//it's the last skill since we just added it

    _c->setSkillParam(skillIndex, "level", QString::number(input->level->value()));
    _c->setSkillParam(skillIndex, "special",  input->special->itemData(input->special->currentIndex()).toString());
    _c->updatePerkNumber(skillIndex, _c->getPerkNumberForSkill(skillIndex));

    updateSkillGUI();
    checkSkillPrerequisite(false);
    updatePointValue();
    _dialogAddSkill->close();
}

void CGenerator::removeSkillFromDialog()
{
    qint32 skillIndex = _dialogRemoveSkill->nameI->currentIndex();
    _c->removeSkillFromIndex(skillIndex);
    updateSkillGUI();
    checkSkillPrerequisite(false);
    updatePointValue();

    _dialogRemoveSkill->close();
}

/*
 * based on _c->_skills, it creates the input for the skills( in _skillI), the grid layout for the skills
 * (in _skillInputLayout), the box for the grid (in _skillInputBox), and put them inside _skillGrid
 * */

void CGenerator::updateSkillGUI()
{
    //First we remove
    int skillNum=_skillI.size();
    for(int i=0;i<skillNum;i++)
    {
        delete(_skillI.takeAt(0));
        delete(_skillInputLayout.takeAt(0));
        delete(_skillInputBox.takeAt(0));
    }

    //Clean skillGrid
    QLayoutItem *child;
    while ((child = _skillGrid->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }
    delete _skillGrid;

    _skillGrid = new QGridLayout;
    for(int i=0;i<_c->getSkill().size();i++)
    {
        CUSkill *skill = _c->getSkill()[i];
        QPointer<QGridLayout> tmpLayout = new QGridLayout;
        QPointer<QGroupBox> tmpBox = new QGroupBox;
        //Recreate new input
        QPointer<CSkillInput> tmpInput = new CSkillInput(d->getSkillNameList(), d->getSkillIdList(), i);
        tmpInput->createPerkInput(d->getUsablePerk(skill->getParam("id"), skill->getParam("level").toInt()),
                                  _c->getPerkNumberForSkill(i), i, skill);
        tmpInput->updateInput(skill);
        _skillI.append(tmpInput);
        //Add everything to the gridlayout
        tmpLayout->addWidget(new QLabel("Nom"), 0, 0);
        tmpLayout->addWidget(new QLabel("Niveau"), 0, 1);
        tmpLayout->addWidget(new QLabel("Spécial"), 0, 2);
        tmpLayout->addWidget(new QLabel("Vitesse"), 0, 3);
        tmpLayout->addWidget(new QLabel("Type"), 0, 4);
        tmpLayout->addWidget(_skillI[i]->level, 1, 1);
        tmpLayout->addWidget(_skillI[i]->special, 1, 2);
        tmpLayout->addWidget(_skillI[i]->speed, 1, 3);
        tmpLayout->addWidget(_skillI[i]->type, 1, 4);
        tmpLayout->addWidget(_skillI[i]->name,1, 0);
        //Now we add the perks (they have been filled with createPerkInput() earlier
        for(int j=0;j<_skillI[i]->perkList.size();j++)
        {
            CPerk *perk = skill->getPerkB()[j];
            _skillI[i]->updatePerk(j, perk);
            tmpLayout->addWidget(_skillI[i]->perkList[j], 2+j, 0);
            tmpLayout->addWidget(_skillI[i]->perkLevel[j], 2+j, 1);
            tmpLayout->addWidget(_skillI[i]->perkDesc[j], 2+j, 2, 1, -1);
            connect(_skillI[i]->perkList[j], SIGNAL(perkDataChanged(qint32, qint32)), this, SLOT(onPerkChanged(qint32, qint32)));
        }
        //I'm not actually sure i need to access those later, but just to be sure i'll put them somewhere
        _skillInputLayout.append(tmpLayout);
        _skillInputBox.append(tmpBox);
        _skillInputBox[i]->setLayout(_skillInputLayout[i]);
        if(i%2 == 0)//We want to put 2 skills by line (cuz it's fancier), so ... different case
        {
            _skillGrid->addWidget(_skillInputBox[i], i/2, 0, 1, 1, Qt::AlignTop);
        }
        else
        {
            _skillGrid->addWidget(_skillInputBox[i], (i-1)/2, 1, 1 , 1, Qt::AlignTop);
        }
        /*fuck that shit
        connect(_skillI[i]->name, SIGNAL(dataChanged(qint32)), this, SLOT(onSkillChanged(qint32)));
        connect(_skillI[i]->level, SIGNAL(dataChanged(qint32)), this, SLOT(onSkillChanged(qint32)));
        connect(_skillI[i]->special, SIGNAL(dataChanged(qint32)), this, SLOT(onSkillChanged(qint32)));
        */
    }
    _skillBox->setLayout(_skillGrid);
}

void CGenerator::onSkillChanged(qint32 skillIndex)
{
        QString newId = _skillI[skillIndex]->name->itemData(_skillI[skillIndex]->name->currentIndex()).toString();
        _c->updateSkillFromIndex(skillIndex, newId);
        CSkillInput *input;
        input = _skillI[skillIndex];
        _c->setSkillParam(skillIndex, "level", QString::number(input->level->value()));
        _c->setSkillParam(skillIndex, "special",  input->special->itemData(input->special->currentIndex()).toString());
        _c->updatePerkNumber(skillIndex, _c->getPerkNumberForSkill(skillIndex));
        updateSkillGUI();
        checkSkillPrerequisite(false);
        updatePointValue();
}

void CGenerator::checkChange()
{
    for(int i=0;i<_skillI.size();i++)
    {
        if(_skillI[i]->name->itemData(_skillI[i]->name->currentIndex()) != _c->getSkill()[i]->getParam("id")
           || _skillI[i]->level->value() != _c->getSkill()[i]->getParam("level").toInt()
           || _skillI[i]->special->itemData(_skillI[i]->special->currentIndex()) != _c->getSkill()[i]->getParam("special"))
        {
            onSkillChanged(i);
        }
    }
}

void CGenerator::onPerkChanged(qint32 skillIndex, qint32 perkIndex)
{
    QString id = _skillI[skillIndex]->perkList[perkIndex]->itemData(_skillI[skillIndex]->perkList[perkIndex]->currentIndex()).toString();
    _c->updatePerkFromIndex(skillIndex, perkIndex, id);
    _skillI[skillIndex]->updatePerk(perkIndex, d->getPerkById(id));
    checkSkillPrerequisite(false);
}

void CGenerator::displayCharacter()
{
    qDebug() << "name (race) : " << _c->getName() << "(" + d->getRaceById(_c->getRaceId())->getName() + ")";
    qDebug() << "health : " << _c->getAttribute("health");
    qDebug() << "strength : " << _c->getAttribute("strength");
    qDebug() << "agility : " << _c->getAttribute("agility");
    qDebug() << "reflexe : " << _c->getAttribute("reflexe");
    qDebug() << "willpower : " << _c->getAttribute("willpower");
    qDebug() << "mana : " << _c->getAttribute("mana");
    qDebug() << "total points : " << _c->getPointsTotal();
    qDebug() << _c->getSkill().size() << " skill(s) : ";
    for(int i=0;i<_c->getSkill().size();i++)
    {
        CUSkill *skill = _c->getSkill()[i];
        QString output = skill->getParam("name") + " " + skill->getParam("level");;
        if(skill->getParam("special")=="t")
        {
            output += " (talent)";
        }
        if(skill->getParam("special")=="g")
        {
            output += " (don)";
        }
        qDebug() << output;
        for(int j=0;j<skill->getPerkB().size();j++)
        {
            qDebug() <<"    " + skill->getPerkB()[j]->getParamStr("name") + " (id : "
                    << skill->getPerkB()[j]->getParamStr("id") + ")";
        }
    }
    for(int i=0;i<_c->getReligionPowerList().size();i++)
    {
        religionPower power = _c->getReligionPowerList()[i];
        qDebug() << power.name << "(id :" << power.id << ") culte : " << power.cult;
    }

}

CCharacter *CGenerator::getCharacter()
{
    return _c;
}

void CGenerator::resetSheet()
{
    CCharacter *tmp = new CCharacter(d);
    delete _c;
    _c = tmp;
    _raceI->setCurrentIndex(0);
    onRaceChanged();
    onNameChanged();
    onAttributeChanged();
    updateSkillGUI();
    updatePointValue();
}

CSkillInput::CSkillInput(QStringList skillName, QStringList skillId, qint32 skillIndex)
{
    name = new CQComboBox;
    for(int i=0;i<skillName.size();i++)
    {
        name->addItem(skillName[i], skillId[i]);
    }
    name->setSkillIndex(skillIndex);
    speed = new QComboBox;
    speed->addItem("Rapide","f");
    speed->addItem("Moyen", "m");
    speed->addItem("Lent", "s");
    speed->setEnabled(false);
    type = new QComboBox;
    type->addItem("Martial", "w");
    type->addItem("Magique", "m");
    type->addItem("Divers", "d");
    type->setEnabled(false);
    special = new CQComboBox;
    special->addItem("Rien", "n");
    special->addItem("Talent", "t");
    special->addItem("Don", "g");
    special->setSkillIndex(skillIndex);
    level = new CQSpinBox;
    level->setMaximum(10);
    level->setSkillIndex(skillIndex);
}

CSkillInput::~CSkillInput()
{
    //this->name->disconnect();
}

void CSkillInput::createPerkInput(QList<CPerk *> perks, qint32 perkNumber, qint32 skillIndex, CUSkill *skill)
{
    //First we need to make a list of the perks name
    if(!skill->isMisc())
    {
        for(int i=0;i<perkNumber;i++)
        {
            CPerkBox *tmpList = new CPerkBox;
            for(int j=0;j<perks.size();j++)
            {
                if(i+1 >= perks[j]->getParamInt("level"))
                {
                    tmpList->addItem(perks[j]->getParamStr("name"), perks[j]->getParamStr("id"));
                }
            }
            tmpList->addItem("", "-1");
            tmpList->setPerkIndex(i);
            tmpList->setSkillIndex(skillIndex);
            QSpinBox *tmpLevel = new QSpinBox;
            QLineEdit *tmpDesc = new QLineEdit;
            tmpLevel->setValue(perks[0]->getParamInt("level"));
            tmpLevel->setReadOnly(true);
            tmpDesc->setText(perks[0]->getParamStr("desc"));
            tmpDesc->setToolTip(perks[0]->getParamStr("desc"));
            perkList << tmpList;
            perkLevel << tmpLevel;
            perkDesc << tmpDesc;
        }
    }
    else if(skill->getParam("id")=="020048")//Technologie. X_x
    {
        for(int i=0;i<perkNumber;i++)
        {
            CPerkBox *tmpList = new CPerkBox;
            for(int j=0;j<perks.size();j++)
            {
                if((perks[j]->getParamInt("level") == 0 && i==0)
                        || (perks[j]->getParamInt("level")==10 && j==perks.size()-1))
                {
                    tmpList->addItem(perks[j]->getParamStr("name"), perks[j]->getParamStr("id"));
                }
                if(2*i-1 == perks[j]->getParamInt("level") && i!=0)
                {
                    tmpList->addItem(perks[j]->getParamStr("name"), perks[j]->getParamStr("id"));
                }
            }
            tmpList->setPerkIndex(i);
            tmpList->setSkillIndex(skillIndex);
            QSpinBox *tmpLevel = new QSpinBox;
            QLineEdit *tmpDesc = new QLineEdit;
            tmpLevel->setValue(perks[0]->getParamInt("level"));
            tmpLevel->setReadOnly(true);
            tmpDesc->setText(perks[0]->getParamStr("desc"));
            tmpDesc->setToolTip(perks[0]->getParamStr("desc"));
            perkList << tmpList;
            perkLevel << tmpLevel;
            perkDesc << tmpDesc;
        }
    }
    else //others Misc skill
    {
        for(int i=0;i<perkNumber;i++)
        {
            CPerkBox *tmpList = new CPerkBox;
            for(int j=0;j<perks.size();j++)
            {

                if(2*i+1 == perks[j]->getParamInt("level") ||
                        (perks[j]->getParamInt("level")==10 && i==4) )
                {
                    tmpList->addItem(perks[j]->getParamStr("name"), perks[j]->getParamStr("id"));
                }
            }
            tmpList->setPerkIndex(i);
            tmpList->setSkillIndex(skillIndex);
            QSpinBox *tmpLevel = new QSpinBox;
            QLineEdit *tmpDesc = new QLineEdit;
            tmpLevel->setValue(perks[0]->getParamInt("level"));
            tmpLevel->setReadOnly(true);
            tmpDesc->setText(perks[0]->getParamStr("desc"));
            tmpDesc->setToolTip(perks[0]->getParamStr("desc"));
            perkList << tmpList;
            perkLevel << tmpLevel;
            perkDesc << tmpDesc;
        }
    }
}

void CSkillInput::updatePerk(qint32 perkIndex, CPerk *perk)
{
    perkList[perkIndex]->setCurrentIndex(perkList[perkIndex]->findData(perk->getParamStr("id")));
    perkDesc[perkIndex]->setText(perk->getParamStr("desc"));
    perkLevel[perkIndex]->setValue(perk->getParamInt("level"));
}

void CSkillInput::updateInput(CUSkill *skill)
{
    name->setCurrentIndex(name->findData(skill->getParam("id")));
    level->setValue(skill->getParam("level").toInt());
    speed->setCurrentIndex(speed->findData(skill->getParam("speed")));
    special->setCurrentIndex(special->findData(skill->getParam("special")));
    type->setCurrentIndex(type->findData(skill->getParam("type")));
}

CQComboBox::CQComboBox()
{
    setSkillIndex(0);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(dummyToEmitSignal()));
}
void CQComboBox::setSkillIndex(qint32 value)
{
    _skillIndex=value;
}

void CQComboBox::dummyToEmitSignal()
{
    emit skillDataChanged(_skillIndex);
}

CQSpinBox::CQSpinBox()
{
    setSkillIndex(0);
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(dummyToEmitSignal()));
}
void CQSpinBox::setSkillIndex(qint32 value)
{
    _skillIndex=value;
}

void CQSpinBox::dummyToEmitSignal()
{
    emit skillDataChanged(_skillIndex);
}

CPerkBox::CPerkBox()
{
    setSkillIndex(0);
    setPerkIndex(0);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(dummyToEmitSignal()));
}
void CPerkBox::setSkillIndex(qint32 value)
{
    _skillIndex=value;
}

void CPerkBox::setPerkIndex(qint32 value)
{
    _perkIndex=value;
}

void CPerkBox::dummyToEmitSignal()
{
    emit perkDataChanged(_skillIndex, _perkIndex);
}
