/*
Copyright (C) 2015 Guillaume DUPUY <glorf@glorf.fr>
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
#include <QtWidgets>
#include "random_generator.h"
#include "random.h"
#include "util.h"
CRandomGenerator::CRandomGenerator(CData *data) : _d(data)
{
    _r = new CRandom(_d);
    _layout = new QGridLayout;
    _attributePoints = new QSpinBox;
    _attributePoints->setMaximum(100000);
    _skillPoints = new QSpinBox;
    _skillPoints->setMaximum(100000);
    _layout->addWidget(new QLabel("Points de caractéristiques (+race)"), 0 , 0);
    _layout->addWidget(_attributePoints, 0, 1);
    _randomRace = new QCheckBox("Race aléatoire");
    _randomRace->setChecked(true);
    _layout->addWidget(_randomRace, 1, 0);
    _layout->addWidget(new QLabel("Points de compétences"), 2 , 0);
    _layout->addWidget(_skillPoints, 2, 1);
    _useWeapon = new QCheckBox("Utiliser des compétences martiales");
    _useWeapon->setChecked(true);
    _useMagic = new QCheckBox("Utiliser des compétences magiques");
    _useMagic->setChecked(true);
    _useMisc = new QCheckBox("Utiliser des compétences diverses");
    _useMisc->setChecked(true);
    _layout->addWidget(_useWeapon, 3, 0);
    _layout->addWidget(_useMagic, 4, 0);
    _layout->addWidget(_useMisc, 5, 0);
    _minLevel = new QSpinBox;
    _minLevel->setValue(1);
    _minLevel->setMaximum(10);
    _maxLevel = new QSpinBox;
    _maxLevel->setValue(10);
    _maxLevel->setMaximum(10);
    _layout->addWidget(new QLabel("Niveau minimum"), 6, 0);
    _layout->addWidget(_minLevel, 6, 1);
    _layout->addWidget(new QLabel("Niveau maximum"), 7, 0);
    _layout->addWidget(_maxLevel, 7, 1);

    _buttonGenerate = new QPushButton("Générer une fiche aléatoire");
    connect(_buttonGenerate, SIGNAL(clicked()), this, SLOT(onActionGenerateSheet()));
    _layout->addWidget(_buttonGenerate, 8, 0, -1, -1);
    //Little hack to make everything not stretched (it's ugly)
    QGroupBox *tmpBox = new QGroupBox;
    tmpBox->setLayout(_layout);
    QVBoxLayout *tmpLayout = new QVBoxLayout;
    tmpLayout->addWidget(tmpBox, 0, Qt::AlignTop);
    setLayout(tmpLayout);
}

CCharacter* CRandomGenerator::randomCharacter(qint32 attributePoints, qint32 skillPoints, qint32 min_lvl, qint32 max_lvl, bool race, bool weapon, bool magic, bool misc)
{
    CCharacter *c = new CCharacter(_d);
    bool assignRace = true;
    if (!race) {
        c->updateRace(_d->getFullId("race", 0));
        assignRace = false;
    }
    qint32 attrib = attributePoints;
    //Random race (just check we have enough points to take it)
    while (assignRace)
    {
        CRace *race = _r->randomRace();
        if (race->getAttribute("cost") <= attrib)
        {
            c->updateRace(race->getId());
            attrib -= race->getAttribute("cost");
            assignRace = false;
        }
    }
    while (attrib >= 10)
    {
        qint32 attr = Util::randInt(0, 5);
        qint32 value = Util::randInt(1,4);
        if(attrib >= value * _d->getCaracCost()[attr]) {
            attrib -= value * _d->getCaracCost()[attr];
            if (attr == 0) //health
                value *= 2;
            if (attr == 5)
                value *= 3;
            c->setAttributeById(attr, c->getAttributeById(attr) + value);
        }
    }

    qint32 sk = skillPoints;

    while (sk >= 20)
    {
        CSkill *skill = _r->randomSkill(min_lvl, max_lvl);
        qint32 cost = _d->getSkillCost(skill->getParam("speed"), skill->getParam("level").toInt());
        if  ( (skill->isWeapon() && !weapon) || (skill->isMagic() && !magic) || (skill->isMisc() && !misc) )
            continue;
        if (sk >= cost)
        {
            qint32 ind = c->getSkill().size();
            c->addSkill(skill->getParam("id"));
            c->setSkillParam(ind, "level", skill->getParam("level"));
            c->setSkillParam(ind, "special", skill->getParam("special"));
            c->updatePerkNumber(ind, c->getPerkNumberForSkill(ind));
            sk -= cost;
        }
    }
    return c;
}

void CRandomGenerator::onActionGenerateSheet()
{
    emit generateSheet(randomCharacter(_attributePoints->value(), _skillPoints->value(), _minLevel->value(), _maxLevel->value(),
                       _randomRace->isChecked(), _useWeapon->isChecked(), _useMagic->isChecked(), _useMisc->isChecked()));
}
