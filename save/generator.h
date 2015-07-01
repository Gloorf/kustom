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
#ifndef GENERATOR_H
#define GENERATOR_H
#include <QtGui>
#include <QApplication>
#include "data.h"
#include "odt_writer.h"
class CDialogAddSkill;
class CDialogRemoveSkill;
class CDialogInfoSkill;
class CDialogReligion;
struct skillInput
{
    QComboBox *name;
    QComboBox *speed;
    QComboBox *type;
    QComboBox *special;
    QSpinBox *level;
    QList<QComboBox *> perkList;
    QList<QSpinBox *> perkLevel;
    QList<QLineEdit *> perkDesc;
};

class CVerticalScrollArea : public QScrollArea
{
    Q_OBJECT
    public:
    explicit CVerticalScrollArea(QWidget *widget);

    virtual bool eventFilter(QObject *o, QEvent *e);

};
class CGenerator : public QWidget
{
    Q_OBJECT
public:
    CGenerator(CData *data);
    CData *d;
    CDialogAddSkill *_dialogNewSkill;
    CDialogRemoveSkill *_dialogRemoveSkill;
    CDialogInfoSkill *_dialogInfoSkill;
    CDialogReligion *_dialogReligion;
    COdtWriter *writer;

    QList<CSkill *> getSkillD() const;
    QList<QList<bool > > getReligionPowerChoice() const;
    void setReligionPowerChoice(int i, int j, bool value);
private:

    QGridLayout *personalGrid;
    QGridLayout *skillGrid;
    QGridLayout *caracGrid;
    QVBoxLayout *generatorLayout;
    QHBoxLayout *topLayout;//Personal + Carac
    QHBoxLayout *buttonLayout;
    QGroupBox *personalBox;
    QGroupBox *skillBox;
    QGroupBox *caracBox;
    QGroupBox *generatorBox;
    QGroupBox *topBox;
    QGroupBox *buttonBox;
    CVerticalScrollArea *skillScrollArea;
    QList<QGridLayout *> skillInputLayout;
    QList<QGroupBox *> skillInputBox;

    QPushButton *buttonFont;
    QPushButton *buttonAddSkill;
    QPushButton *buttonRemoveSkill;
    QPushButton *buttonInfoSkill;
    QPushButton *buttonGenerate;
    QPushButton *buttonReset;
    QPushButton *buttonReligion;

    //Personal input
    QLineEdit *charName;
    QComboBox *raceI;
    QSpinBox *pointValue;
    QSpinBox *advantageValue;
    QLineEdit *fontName;
    //Skill input


    QList<skillInput> skillI;
    //carac input
    QList<QSpinBox *> caracI;
    QList<QSpinBox *> caracB;

    //Based on input
    qint32 selectedRaceIndex;
    QString selectedRace;
    QList<CSkill *> skillD;
    charData charD;
    charData charB;
    QList<bool> perkNumberOkay;
    QFont usedFont;
    QList<QList<bool > > religionPowerChoice;
    QList<religionPower> religionPowerD;

    //Function
    void createPersonalBox();
    void createSkillBox();
    void createCaracBox();
    void createGeneratorLayout();
    QGroupBox *newSkillBox(QGridLayout *layoutOutput, skillInput input);
    QList<QStringList> newPerkL(qint32 curInd, QList<CPerk *> perkBaseList);
    void updatePerkNumber(qint32 curInd, qint32 realIndex);
    void updatePerkData(qint32 curInd, qint32 realIndex);
    void updateCharData();
    bool checkSkill(bool verbose);
    void calculateCost();
    qint32 raceGiftPoint(bool verbose);
    void removeSkillAtInd(qint32 index);

private slots:
    void updateCarac();
    void generateSheet();
    void addSkillToLayout();
    void addSkillDialog();
    void removeSkillDialog();
    void religionDialog();
    void infoSkillDialog();
    void removeSkill();
    void updateSkill();
    void updateFont();
    void resetSheet();
    void religionEffect();

signals:
    void sendResetGenerator();
};



#endif // GENERATOR_H
