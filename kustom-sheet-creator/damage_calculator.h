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
#ifndef DAMAGE_CALCULATOR_H
#define DAMAGE_CALCULATOR_H
#include <QtGui>
#include <QtWidgets>

#include <QApplication>
#include "data.h"
class CDamageCalculator : public QWidget
{
    Q_OBJECT
public:
    CDamageCalculator(CData *data);
    CData *d;
private:
    //Taken from simulator.h ; need to find a cool way so i don't have to redefine it
    struct diceData
    {
        qint32 value;//Max of the dice
        qint32 roll;//How many time we roll a dice
        qint32 bonus;//+C
    };
    qint32 diceRoll(damageData dice);
    qint32 randInt(qint32 low, qint32 high);
    void createPersonalBox();
    void createBodyPartBox();
    void createDamageCalculatorLayout();
    bool updateDamageData();//Based on damageB, & damageUser,  we fill damageD
    bool updateDamageUser();//Fill damageUser &DamageUserSup based on damageI
    bool isDice(QString rawText);

    QGroupBox *personalBox;//User dmg, etc

    QList<QGroupBox *>  bodyPartBox;
    QList<QGridLayout *> bodyPartGrid;

    QGridLayout *personalGrid;
    QGridLayout *damageCalculatorLayout;
    QPushButton *buttonCalculate;

    //Input
    QLineEdit *damageI;
    QSpinBox *damageBonusI;
    QLineEdit *damageSupI;
    struct bodyZoneInput
    {
        QLabel *name;
        QSpinBox *value;//Value of the dice roll
        QLineEdit *stat;//Here is Min-Max[Moy]
    };
    //First QList is the part (head ...), second is all the zone in this part (skull, face...)
    QList<QList<bodyZoneInput> >bodyI;
    //Data
    QList<QList<QList<damageData> > > damageD;
    QList<damageData> damageUser;//Weapon damage, eg 1d10+7
    QList<damageData> damageUserSup;//sup damage, like +1d10 for coup brutal
private slots:
    void calculateDamage();
};

#endif // DAMAGE_CALCULATOR_H
