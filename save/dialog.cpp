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

CDialogReligion::CDialogReligion(CGenerator *_generator)
{
    /*
    g=_generator;
    d=g->d;
    QWidget *viewport = new QWidget(this);

    QScrollArea *scroll = new CVerticalScrollArea(viewport);

    dialogLayout = new QVBoxLayout(viewport);
    buttonDone = new QPushButton("Fermer", this);
    connect(buttonDone, SIGNAL(clicked()), this, SLOT(checkCompatibility()));
    for(int i=0;i<d->getReligionB().size();i++)
    {
        god << new QLabel(d->getGodName()[i]);
        dialogLayout->addWidget(god[i]);
        QTableWidget *tmpTable = new QTableWidget(d->getReligionB()[i]->size(), 3, this);
        QStringList headers;
        headers << "Nom" << "Coût" << "Le prendre ?";
        tmpTable->setHorizontalHeaderLabels(headers);
        QList<QCheckBox *> tmpList;
        choice << tmpList;
        for(int j=0;j<d->getReligionB()[i]->size();j++)
        {
            religionPower power = d->getReligionB()[i][j];
            tmpTable->setItem(j,0, new QTableWidgetItem(power.name));
            tmpTable->setItem(j, 1, new QTableWidgetItem(QString::number((power.cost))));
            if(power.cult)
            {
                tmpTable->item(j, 0)->setForeground(Qt::red);
            }
            choice[i] << new QCheckBox(this);
            choice[i][j]->setChecked(g->getReligionPowerChoice()[i][j]);
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
        tmpTable->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        table << tmpTable;
        dialogLayout->addWidget(table[i]);
    }
    QVBoxLayout *dialog2 = new QVBoxLayout(this);
    dialog2->addWidget(scroll);
    dialog2->addWidget(buttonDone);
    setLayout(dialog2);
    setAttribute(Qt::WA_DeleteOnClose);
    show();
    */
}

CDialogReligion::~CDialogReligion()
{
    /*
    for(int i=0;i<g->getReligionPowerChoice().size();i++)
    {
        for(int j=0;j<g->getReligionPowerChoice()[i].size();j++)
        {
            g->setReligionPowerChoice(i,j,choice[i][j]->isChecked());
        }
    }
    */
}

void CDialogReligion::checkCompatibility()
{
    /*
    QString output="";
    QList<qint32> godChosen;
    //i is the ind for the god we are checking, j for the specifics power,
    for(int i=0;i<g->getReligionPowerChoice().size();i++)
    {
        for(int j=0;j<g->getReligionPowerChoice()[i].size();j++)
        {
            if(choice[i][j]->isChecked() && d->getReligionB()[i][j].cult //Cult needed by power but not taken
                    && !choice[i][0]->isChecked())
            {
                output += "Vous avez pris le pouvoir " + d->getReligionB()[i][j].name
                + " sans avoir pris le " + d->getReligionB()[i][0].name + ".\n";
            }
            if(choice[i][j]->isChecked())
            {
                godChosen << i;
            }

        }
    }
    for(int i=0;i<godChosen.size();i++)
    {
        for(int j=0;j<godChosen.size();j++)
        {
            if(d->getReligionCompatibility()[godChosen[i]][godChosen[j]]=="t")
            {
                if(choice[godChosen[i]][0]->isChecked() && choice[godChosen[j]][0]->isChecked())//We have both cult, wrong !
                {
                    QString tmp = "Vous avez pris le culte de " + d->getGodName()[godChosen[i]] + " et celui de "
                            + d->getGodName()[godChosen[j]] + " alors qu'ils ne sont que tolérants.\n";
                    QString tmp2 = "Vous avez pris le culte de " + d->getGodName()[godChosen[j]] + " et celui de "
                            + d->getGodName()[godChosen[i]] + " alors qu'ils ne sont que tolérants.\n";
                    //Checking for duplication before adding them
                    if(output.indexOf(tmp) == -1 && output.indexOf(tmp2) == -1)
                    {
                        output += tmp;
                    }
                }
                //Else we have powers in both but it's okay
            }
            //Incompatibility, can't get power of both gods
            if(d->getReligionCompatibility()[godChosen[i]][godChosen[j]]=="i")
            {
                QString powerL="";
                QString powerL2="";
                for(int k=0;k<d->getReligionB()[godChosen[i]].size();k++)
                {
                    if(choice[godChosen[i]][k]->isChecked())
                    {
                        powerL += d->getReligionB()[godChosen[i]][k].name + ", ";
                    }
                }
                for(int k=0;k<d->getReligionB()[godChosen[j]].size();k++)
                {
                    if(choice[godChosen[j]][k]->isChecked())
                    {
                        powerL2 += d->getReligionB()[godChosen[j]][k].name + ", ";
                    }
                }
                //Remove the extra ", "
                powerL.chop(2);
                powerL2.chop(2);
                QString tmp = "Vous avez des pouvoirs à la fois d'" + d->getGodName()[godChosen[i]] + " (" + powerL + ") et d'"
                        + d->getGodName()[godChosen[j]] + " (" + powerL2 + "), qui sont incompatibles.\n";
                QString tmp2 = "Vous avez des pouvoirs à la fois d'" + d->getGodName()[godChosen[j]] + " (" + powerL2 + ") et d'"
                        + d->getGodName()[godChosen[i]] + " (" + powerL + "), qui sont incompatibles.\n";

                if(output.indexOf(tmp) == -1 && output.indexOf(tmp2) == -1)
                {

                    output += tmp;
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
    */
}

CDialogInfoSkill::CDialogInfoSkill(CGenerator *_g)
{
    /*
    g=_g;
    d=g->d;
    inputLayout = new QGridLayout;
    dialogLayout = new QVBoxLayout;
    inputBox = new QGroupBox;
    nameI=new QComboBox;
    for(int i=0;i<g->getSkillD().size();i++)
    {
        QString tmpName = _g->getSkillD()[i]->getInfo().name +" - " + QString::number(_g->getSkillD()[i]->getInfo().level);
        tmpName= _g->getSkillD()[i]->getInfo().special == 21 ? tmpName + " (Talent)" : tmpName;
        tmpName= _g->getSkillD()[i]->getInfo().special == 22 ? tmpName + " (Don)" : tmpName;
        nameI->addItem(tmpName);
    }
    nameBI = new QComboBox;
    for(int i=0;i<d->getSkillB().size();i++)
    {
        QString tmpName = d->getSkillB()[i]->getInfo().name + " (Base) ";
        nameBI->addItem(tmpName);
    }
    levelBI = new QSpinBox;
    levelBI->setMinimum(0);
    levelBI->setValue(10);
    levelBI->setMaximum(10);
    tableType = new QComboBox;
    tableType->addItem("Compétences personnalisées");
    tableType->addItem("Compétences de base");
    buttonDone = new QPushButton("Fermer", this);
    connect(buttonDone, SIGNAL(clicked()), this, SLOT(close()));
    connect(tableType, SIGNAL(currentIndexChanged(int)), this, SLOT(updateDisplayedChoice()));
    connect(nameI, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTable()));
    connect(nameBI, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTable()));
    connect(levelBI, SIGNAL(valueChanged(int)), this, SLOT(updateTable()));
    inputLayout->addWidget(new QLabel("Quel type de tableau afficher ?"),0,0);
    inputLayout->addWidget(tableType, 0, 1);
    inputLayout->addWidget(nameI, 1, 0, 1, -1);
    inputLayout->addWidget(nameBI,2,0);
    inputLayout->addWidget(levelBI, 2, 1);
    inputBox->setLayout(inputLayout);
    dialogLayout->addWidget(inputBox);
    setLayout(dialogLayout);
    updateDisplayedChoice();
    show();
    */

}
/*
 * */
void CDialogInfoSkill::updateDisplayedChoice()
{

    /*
    if(g->getSkillD().size()==0 && tableType->currentIndex()==0)
    {
        tableType->setCurrentIndex(1);
        QMessageBox::warning(this, "Pas de compétence personnalisée",
        "impossible d'afficher la liste des compétences personnalisées car il n'y en a aucune sur la fiche.");
    }
    if(tableType->currentIndex()==0)
    {
        nameI->setVisible(true);
        nameBI->setVisible(false);
        levelBI->setVisible(false);
    }
    if(tableType->currentIndex()==1)
    {
        nameI->setVisible(false);
        nameBI->setVisible(true);
        levelBI->setVisible(true);
    }

    updateTable();
    */
}

/*
 * */
void CDialogInfoSkill::updateTable()
{
    /*
   /* QLayoutItem *child;
    while ((child = dialogLayout->takeAt(0)) != 0)
    {
        delete child;
    }
    delete dialogLayout;
    QVBoxLayout *newLayout = new QVBoxLayout;
    newLayout->addWidget(inputBox);
    dialogLayout=newLayout;
    if(table != 0)
    {
        delete table;
    }
    dialogLayout->removeWidget(table);
    dialogLayout->removeWidget(buttonDone);
    adjustSize();
    if(tableType->currentIndex()==0)
    {
        qint32 realIndex=d->findSkillIndex(g->getSkillD()[nameI->currentIndex()]->getInfo().name);
        CSkill *tmpSkill=d->getSkillB()[realIndex];
        skillData tmpData = tmpSkill->getInfo();
        tmpData.level=g->getSkillD()[nameI->currentIndex()]->getInfo().level;
        tmpSkill->setInfo(tmpData);
        table=createTable(tmpSkill);
    }
    else if(tableType->currentIndex()==1)
    {
        CSkill *tmpSkill=d->getSkillB()[nameBI->currentIndex()];
        skillData tmpData = tmpSkill->getInfo();
        tmpData.level=levelBI->value();
        tmpSkill.setInfo(tmpData);
        table=createTable(&tmpSkill);
    }
    dialogLayout->addWidget(table);

    dialogLayout->addWidget(buttonDone);

    */
}

/*
 * */
QTableWidget* CDialogInfoSkill::createTable(CSkill *skill)
{
    /*
    if(skill->getInfo().name=="Forge technomagique")
    {
        return new QTableWidget;
    }
    QTableWidget* output = new QTableWidget(skill->rowCount(),skill->columnCount(), this);
    output->setHorizontalHeaderLabels(skill->headerLabels());
    if(skill->isMagic())
    {
        for(int i=0;i<output->rowCount();i++)
        {
            CPerk *perk=skill->getPerkB()[i];
            QString tmpName=perk.name + "\n" + QString::number(perk.cost) + "pts" ;
            QString tmpDesc = wordWrap(perk.desc, 60);
            QString tmpMisc = wordWrap(perk.misc, 60);
            tmpMisc = perk.slowIncant ? tmpMisc + " Incantation lente." : tmpMisc;
            tmpMisc = perk.fastIncant ? tmpMisc + " Incantation instantanée." : tmpMisc;
            tmpMisc = perk.focus ? tmpMisc + " Jet de concentration requis à chaque tour." : tmpMisc;
            output->setItem(i,0,new QTableWidgetItem(tmpName));

            if(skill->getInfo().name=="Moniale")
            {
                output->setItem(i,1,new QTableWidgetItem(perk.school));
                output->setItem(i,2,new QTableWidgetItem(perk.dd));
                output->setItem(i,3,new QTableWidgetItem(perk.condition));
                output->setItem(i,4,new QTableWidgetItem(tmpDesc));
                output->setItem(i,5,new QTableWidgetItem(perk.cooldown));
                output->setItem(i,6,new QTableWidgetItem(tmpMisc));
            }
            else if(skill->multipleSchool())
            {
                output->setItem(i,1,new QTableWidgetItem(perk.school));
                output->setItem(i,2,new QTableWidgetItem(perk.dd));
                output->setItem(i,3,new QTableWidgetItem(tmpDesc));
                output->setItem(i,4,new QTableWidgetItem(perk.cooldown));
                output->setItem(i,5,new QTableWidgetItem(tmpMisc));
            }
            else if(skill->getInfo().name=="Sceaux")
            {
                output->setItem(i,1,new QTableWidgetItem(perk.dd));
                output->setItem(i,2,new QTableWidgetItem(perk.incantTime));
                output->setItem(i,3,new QTableWidgetItem(tmpDesc));
                output->setItem(i,4,new QTableWidgetItem(tmpMisc));

            }
            else if(skill->getInfo().name=="Sorcellerie")
            {
                output->setItem(i,1,new QTableWidgetItem(perk.dd));
                output->setItem(i,2,new QTableWidgetItem(perk.optionalComponent));
                output->setItem(i,3,new QTableWidgetItem(tmpDesc));
                output->setItem(i,4,new QTableWidgetItem(perk.cooldown));
                output->setItem(i,5,new QTableWidgetItem(tmpMisc));
            }
            else
            {
                output->setItem(i,1,new QTableWidgetItem(perk.dd));
                output->setItem(i,2,new QTableWidgetItem(tmpDesc));
                output->setItem(i,3,new QTableWidgetItem(perk.cooldown));
                output->setItem(i,4,new QTableWidgetItem(tmpMisc));
            }
        }
    }//skill->isMagic()
    else if(skill->isWeapon())
    {
        for(int i=0;i<output->rowCount();i++)
        {
            CPerk *perk=skill->getPerkB()[i];
            QString tmpName=perk.name + "\n" + QString::number(perk.cost) + "pts" ;
            QString tmpDesc = wordWrap(perk.desc, 60);
            QString tmpMisc = wordWrap(perk.misc, 60);
            QString tmpNeeded = skill->getInfo().name + " " + QString::number(perk.level);
            tmpNeeded = !perk.levelNeeded.isEmpty() ? tmpNeeded+ "\n" + perk.levelNeeded : tmpNeeded;
            output->setItem(i,0, new QTableWidgetItem(tmpName));
            output->setItem(i,1, new QTableWidgetItem(tmpNeeded));
            output->setItem(i,2, new QTableWidgetItem(tmpDesc));
            output->setItem(i,3, new QTableWidgetItem(perk.cooldown));
            output->setItem(i,4, new QTableWidgetItem(tmpMisc));
        }

    }
    else if(skill->isMisc())
    {
        qint32 count=0;
        for(int i=0;i<output->rowCount();i++)
        {
            CPerk *perk=skill->getPerkB()[count];
            QString tmpName=skill->getInfo().name + " " + QString::number(perk.level);
            output->setItem(i, 0, new QTableWidgetItem(tmpName));
            for(int j=1;j<output->columnCount();j++)
            {
                CPerk *perk=skill->getPerkB()[count];
                QString tmpDesc = wordWrap(perk.name + " : " + perk.desc,40);
                output->setItem(i,j, new QTableWidgetItem(tmpDesc));
                count++;
            }
        }
    }
    output->resizeRowsToContents();
    output->resizeColumnsToContents();
    output->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    int width = output->columnCount() + output->verticalHeader()->width();
    for(int column = 0; column < output->columnCount(); column++)
    {
        width = width + output->columnWidth(column);
    }
    width+=d->getSettings()->value("dialogSuppWidth").value<qint32>();
    output->setMinimumWidth(width);
    output->resizeRowsToContents();
    output->resizeColumnsToContents();
    output->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    output->setEditTriggers(QAbstractItemView::NoEditTriggers);

    return output;
    */
    return NULL;
}
/*
 * */
