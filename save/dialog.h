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

#ifndef DIALOG_H
#define DIALOG_H
#include <QtGui>
#include <QApplication>
#include "data.h"
#include "generator.h"
//class CGenerator;
class CDialogAddSkill : public QDialog
{
Q_OBJECT
public:
    CDialogAddSkill(CGenerator *_generator, qint32 skillIndex);
    CSkillInput *tmpInput;
private:
    QGridLayout *dialogLayout;
    QPushButton *buttonAddSkill;
    QPushButton *buttonCancel;
};
class CDialogReligion : public QDialog
{
    Q_OBJECT
public:
    CDialogReligion(CGenerator *_generator);
    ~CDialogReligion();
    CGenerator *g;
    CData *d;
private:
    QList<QLabel *> god;
    QList<QTableWidget *> table;
    QList<QList<QCheckBox *> > choice;
    QVBoxLayout *dialogLayout;
    CVerticalScrollArea *dialogScroll;
    QWidget *central;
    QPushButton *buttonDone;
private slots:
    void checkCompatibility();

};

class CDialogRemoveSkill : public QDialog
{
    Q_OBJECT
public:
    CDialogRemoveSkill(CGenerator *_generator, QStringList skillName);
    QComboBox *nameI;
private :
    QGridLayout *_dialogLayout;
    QPushButton *_buttonRemoveSkill;
    QPushButton *_buttonCancel;
};

class CDialogInfoSkill : public QDialog
{
    Q_OBJECT
public:
    CDialogInfoSkill(CGenerator *_g);
    CGenerator *g;
    CData *d;
private:
    QTableWidget *table;
    QComboBox *nameI;
    QComboBox *nameBI;
    QSpinBox *levelBI;
    QComboBox *tableType;
    QPushButton *buttonDone;
    QGridLayout *inputLayout;
    QGroupBox *inputBox;
    QVBoxLayout *dialogLayout;
    QTableWidget* createTable(CSkill *skill);
    qint32 skillColumnCount(CSkill *skill);
private slots:
    void updateTable();
    void updateDisplayedChoice();
};
#endif // DIALOG_H
