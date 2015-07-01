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

QString wordWrap(const QString &str, int wrapLength)
{
    QString tempStr=str;
    qint32 line=0;
    for(int i=0;i<str.length();i++)
    {
        if(i>= wrapLength*(line+1) && str.at(i)==' ')
        {
            tempStr.replace(i, 1, "\n");
            line++;
        }
    }
    return tempStr;
}

CDialogAddSkill::CDialogAddSkill(CGenerator *_generator, qint32 skillIndex)
{
    dialogLayout = new QGridLayout;
    tmpInput = new CSkillInput(_generator->d->getSkillNameList(), _generator->d->getSkillIdList(), skillIndex);
    buttonAddSkill = new QPushButton("&Ajouter",this);
    buttonCancel = new QPushButton("A&nnuler", this);
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(buttonAddSkill, SIGNAL(clicked()), _generator,
            SLOT(addSkillFromDialog()));
    dialogLayout->addWidget(new QLabel(tr("Nom")), 0, 0);
    dialogLayout->addWidget(new QLabel(tr("Spécial")),1, 0);
    dialogLayout->addWidget(new QLabel(tr("Niveau")), 2, 0);
    dialogLayout->addWidget(tmpInput->name, 0, 1);
    dialogLayout->addWidget(tmpInput->special, 1, 1);
    dialogLayout->addWidget(tmpInput->level, 2 ,1);
    dialogLayout->addWidget(buttonCancel,3,0);
    dialogLayout->addWidget(buttonAddSkill,3,1);
    setLayout(dialogLayout);
    show();
}

CDialogRemoveSkill::CDialogRemoveSkill( CGenerator *_generator, QStringList skillName)
{

    _dialogLayout = new QGridLayout;
    nameI = new QComboBox;
    nameI->addItems(skillName);
    _buttonRemoveSkill = new QPushButton("Supprimer", this);
    _buttonCancel = new QPushButton("Annuler", this);
    connect(_buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(_buttonRemoveSkill, SIGNAL(clicked()), _generator, SLOT(removeSkillFromDialog()));
    _dialogLayout->addWidget(new QLabel(tr("Compétence à supprimer")), 0, 0, 1, 2);
    _dialogLayout->addWidget(nameI, 1, 0, 1, 2);
    _dialogLayout->addWidget(_buttonCancel, 2, 0);
    _dialogLayout->addWidget(_buttonRemoveSkill, 2, 1);
    setLayout(_dialogLayout);
    show();

}
/*
 * */

CDialogReligion::CDialogReligion(CGenerator *_generator, QList<religionPower> currentPowers)
{
    g=_generator;
    g=_generator;
    d=g->d;
    QWidget *viewport = new QWidget(this);

    QScrollArea *scroll = new CVerticalScrollArea(viewport);

    dialogLayout = new QVBoxLayout(viewport);
    buttonDone = new QPushButton("Fermer", this);
    connect(buttonDone, SIGNAL(clicked()), this, SLOT(checkCompatibility()));
    for(int i=0;i<d->getReligionB().size();i++)
    {
        CReligion *religion =d->getReligionB()[i];
        god << new QLabel(religion->getName());
        dialogLayout->addWidget(god[i]);
        QTableWidget *tmpTable = new QTableWidget(religion->getPowerList().size(), 3, this);
        QStringList headers;
        headers << "Nom" << "Coût" << "Le prendre ?";
        tmpTable->setHorizontalHeaderLabels(headers);
        QList<QCheckBox *> tmpList;
        choice << tmpList;
        for(int j=0;j<religion->getPowerList().size();j++)
        {
            religionPower power = religion->getPowerList()[j];
            tmpTable->setItem(j,0, new QTableWidgetItem(power.name));
            tmpTable->setItem(j, 1, new QTableWidgetItem(QString::number((power.cost))));
            if(power.cult)
            {
                tmpTable->item(j, 0)->setForeground(Qt::red);
            }
            choice[i] << new QCheckBox(this);
            for(int k=0;k<currentPowers.size();k++)
            {
                if(currentPowers[k].id == power.id)
                {
                    choice[i][j]->setChecked(true);
                }
            }
            //choice[i][j]->setChecked(g->getReligionPowerChoice()[i][j]);
            tmpTable->setCellWidget(j,2, choice[i][j]);
        }
        tmpTable->resizeColumnsToContents();
        tmpTable->resizeRowsToContents();
        int width = tmpTable->verticalHeader()->width();
        for(int column = 0; column < tmpTable->columnCount(); column++)
        {
            width = width + tmpTable->columnWidth(column);
        }
        width+=d->getSettings()->value("dialogSuppWidth").value<qint32>();
        tmpTable->setMinimumWidth(width);

        int height = tmpTable->rowCount() + tmpTable->horizontalHeader()->height();
        for(int row = 0; row < tmpTable->rowCount(); row++)
        {
            height = height + tmpTable->rowHeight(row);
        }
        //height+=5;
        tmpTable->setMinimumHeight(height);
        tmpTable->resizeRowsToContents();
        tmpTable->resizeColumnsToContents();
        tmpTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        table << tmpTable;
        dialogLayout->addWidget(table[i]);
    }
    QVBoxLayout *dialog2 = new QVBoxLayout(this);
    dialog2->addWidget(scroll);
    dialog2->addWidget(buttonDone);
    setLayout(dialog2);
    setAttribute(Qt::WA_DeleteOnClose);
    show();

}

CDialogReligion::~CDialogReligion()
{
    g->getCharacter()->resetReligion();
    for(int i=0;i<_powers.size();i++)
    {
        g->getCharacter()->addReligionPower(_powers[i].id);
    }

}

void CDialogReligion::checkCompatibility()
{
    QString output = "";
    QList<CReligion *> religions;
    //First we check for the cult needed, and the list of religion for compatibilities
    for(int i=0;i<choice.size();i++)
    {
        CReligion *religion = d->getReligionB()[i];
        bool hasCult = false;
        for(int j=0;j<choice[i].size();j++)
        {
            religionPower power = religion->getPowerList()[j];
            if (choice[i][j]->isChecked())
            {
                if(power.id == religion->getCult().id)
                {
                    hasCult = true;
                    _powers << power;
                }
                else if((power.cult && hasCult) || (!power.cult))
                {
                    _powers << power;
                }
                else
                {
                    output += "Vous avez pris le pouvoir " + power.name + " sans avoir pris le " + religion->getCult().name +".\n";
                }
                if(!religions.contains(religion))
                {
                    religions << religion;
                }
            }
        }
    }
    //Now compatibilities
    for (int i=0;i<religions.size();i++)
    {
        CReligion *current = religions[i];
        for(int j=0;j<religions.size();j++)
        {
            CReligion *other = religions[j];
            QString value = current->getCompatibilities().value(other->getId());
            if(value == "i")
            {
                QString tmp = "Vous avez pris un (des) pouvoir(s) de " + current->getName() + " et de "
                        + other->getName() +   " alors qu'ils sont incompatibles.\n";
                QString tmp2= "Vous avez pris un (des) pouvoir(s) de " + other->getName()   + " et de "
                        + current->getName() + " alors qu'ils sont incompatibles.\n";
                if(output.indexOf(tmp) == -1 && output.indexOf(tmp2) == -1)
                {
                    output += tmp;
                }
            }
            if(value == "t")//Tolérance
            {
                qint32 nbCult = 0;
                for(int k=0;k<_powers.size();k++)
                {
                    if(_powers[k].id == current->getCult().id || _powers[k].id == other->getCult().id)
                    {
                        nbCult++;
                    }
                }
                if(nbCult > 1)
                {
                    QString tmp = "Vous avez pris le culte de " + current->getName() + " et celui de "
                            + other->getName() + " alors qu'ils ne sont que tolérants.\n";
                    QString tmp2 = "Vous avez pris le culte de " + other->getName() + " et celui de "
                            + current->getName() + " alors qu'ils ne sont que tolérants.\n";
                    if(output.indexOf(tmp) == -1 && output.indexOf(tmp2) == -1)
                    {
                        output += tmp;
                    }
                }
            }
        }
    }
    if(output !="")
    {
        QMessageBox msgBox;
        QPushButton *continueButton = msgBox.addButton(tr("Fermer la fenêtre quand même"), QMessageBox::ActionRole);
        QPushButton *abortButton = msgBox.addButton(tr("Revenir à la liste des pouvoirs"), QMessageBox::NoRole);
        msgBox.setText(output);
        msgBox.exec();
        if (msgBox.clickedButton()==continueButton )
        {
            close();
        }
        else if (msgBox.clickedButton() == abortButton)
        {
            return;
        }
    }
    else
    {
        close();
    }
}

