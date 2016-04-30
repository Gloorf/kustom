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
#include <QtWidgets>
#include "data.h"
#include "data_type.h"
#include "character.h"
#include "odt_writer.h"
class CDialogAddSkill;
class CDialogRemoveSkill;
class CDialogReligion;
class CGenerator;
class CQSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    CQSpinBox();
    void setSkillIndex(qint32 value);
signals:
    void skillDataChanged(qint32 skillIndex);
private slots:
    void dummyToEmitSignal();
private:
    qint32 _skillIndex;
};

class CQComboBox : public QComboBox
{
    Q_OBJECT
public:
    CQComboBox();
    void setSkillIndex(qint32 value);
signals:
    void skillDataChanged(qint32 skillIndex);
private slots:
    void dummyToEmitSignal();
private:
    qint32 _skillIndex;

};

class CPerkBox : public QComboBox
{
    Q_OBJECT
public:
    CPerkBox();
    void setSkillIndex(qint32 value);
    void setPerkIndex(qint32 value);
signals:
    void perkDataChanged(qint32 skillIndex, qint32 perkIndex);
private slots:
    void dummyToEmitSignal();
private:
    qint32 _skillIndex;
    qint32 _perkIndex;
};

class CSkillInput : public QObject
{
    Q_OBJECT
public:
    CSkillInput(QStringList skillName, QStringList skillId, qint32 skillIndex);
    ~CSkillInput();

    void createPerkInput(QList<CPerk *> perks, qint32 perkNumber, qint32 skillIndex, CUSkill *skill);
    void updateInput(CUSkill *skill);

    QPointer<CQComboBox> name;
    QPointer<QComboBox> speed;
    QPointer<QComboBox> type;
    QPointer<CQComboBox> special;
    QPointer<CQSpinBox> level;
    QList<QPointer<CPerkBox > > perkList;
    QList<QPointer<QSpinBox > > perkLevel;
    QList<QPointer<QLineEdit > > perkDesc;
    void updatePerk(qint32 perkIndex, CPerk *perk);
};



class CGenerator : public QWidget
{
    Q_OBJECT
public:
    CGenerator(CData *data);
    CData *d;
    CCharacter *getCharacter();
    void setCharacter(CCharacter *c);
private:
    CCharacter *_c;
    CDialogAddSkill *_dialogAddSkill;
    CDialogRemoveSkill *_dialogRemoveSkill;
    CDialogReligion *_dialogReligion;
    COdtWriter *_writer;
    //Init
    void createPersonalBox();
    void createCaracBox();
    void createSkillBox();
    void createGeneratorLayout();
    //Based solely on the character data
    void updateGUI();
    void updateSkillGUI();
    bool checkSkillPrerequisite(bool verbose);
    void updateCharacterAttribute();
    void updateBaseAttribute();
    void updatePersonal();
    QVBoxLayout *_generatorLayout;
    QHBoxLayout *_topLayout;//For personal + carac boxes
    QGridLayout *_personalGrid;
    QGridLayout *_attributeGrid;
    QGridLayout *_skillGrid;
    QHBoxLayout *_buttonLayout;

    QGroupBox *_topBox;
    QGroupBox *_personalBox;
    QGroupBox *_attributeBox;
    CVerticalScrollArea *_skillScrollArea;
    QGroupBox *_skillBox;
    QGroupBox *_buttonBox;

    QList<QPointer<QGridLayout> > _skillInputLayout;
    QList<QPointer<QGroupBox> > _skillInputBox;

    QPushButton *_buttonFont;
    QPushButton *_buttonDebug;
    QPushButton *_buttonAddSkill;
    QPushButton *_buttonRemoveSkill;
    QPushButton *_buttonReligion;
    QPushButton *_buttonWriteSheet;
    QPushButton *_buttonReset;
    //Input from personalBox
    QLineEdit *_charName;
    QComboBox *_raceI;
    QSpinBox *_pointValue;
    QSpinBox *_advantageValue;
    QSpinBox *_religionValue;
    QLineEdit *_fontNameEdit;
    //input from caracBox
    QList<QSpinBox *> _attributeI;
    QList<QSpinBox *> _attributeB;
    QList<QPointer<CSkillInput> > _skillI;
    QSignalMapper *_mapper_attribute;
    //Based on input
    QFont _usedFont;
private slots:
    void onAttributeChanged(qint32 index);
    void onAdvantageValueChanged();
    void onRaceChanged();
    void onNameChanged();
    void onFontChanged();

    void addSkillDialog();
    void removeSkillDialog();
    void religionDialog();


    void addSkillFromDialog();
    void removeSkillFromDialog();
    void displayCharacter();
    void onSkillChanged(qint32 skillIndex=0);
    void onPerkChanged(qint32 skillIndex=0, qint32 perkIndex=0);

    void writeSheet();
    void checkChange();
    void resetSheet();
public slots:
    void updatePointValue();
};

#endif // GENERATOR_H
