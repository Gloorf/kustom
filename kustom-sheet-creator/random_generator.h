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

#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <QtWidgets>
#include "data.h"
#include "generator.h"
#include "random.h"
class CRandomGenerator : public QWidget
{
    Q_OBJECT

public:
    CRandomGenerator(CData *data);
private:
    CData *_d;
    CRandom *_r;
    QGridLayout *_layout;
    QPushButton *_buttonGenerate;
    QSpinBox *_attributePoints;
    QSpinBox *_skillPoints;
    QSpinBox *_minLevel;
    QSpinBox *_maxLevel;
    QCheckBox *_randomRace;
    QCheckBox *_useWeapon;
    QCheckBox *_useMagic;
    QCheckBox *_useMisc;

    CCharacter* randomCharacter(qint32 attributePoints, qint32 skillPoints, qint32 min_lvl, qint32 max_lvl, bool race, bool weapon, bool magic, bool misc);
private slots:
    void onActionGenerateSheet();
signals:
    void generateSheet(CCharacter *character);
};

#endif // RANDOM_GENERATOR_H

