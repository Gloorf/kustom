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
#include "simulator.h"
#include "damage_calculator.h"
#include "random_generator.h"
CMainWindow::CMainWindow()
{

    tabWidget = new QTabWidget;
    data = new CData;
    generator = new CGenerator(data);
    simulator = new CSimulator;
    damageCalculator = new CDamageCalculator(data);
    randomGenerator = new CRandomGenerator(data);
    bool scrollBar = data->getSettings()->value("scrollBar").value<bool>();
    if(scrollBar)
    {
        generatorScroll=new CVerticalScrollArea(generator);
        simulatorScroll = new CVerticalScrollArea(simulator);
        damageCalculatorScroll = new CVerticalScrollArea(damageCalculator);
        randomGeneratorScroll = new CVerticalScrollArea(randomGenerator);
        tabWidget->addTab(generatorScroll, tr("Générateur de fiche"));
        tabWidget->addTab(simulatorScroll, tr("Simulateur de dés"));
        tabWidget->addTab(damageCalculatorScroll, tr("Calculateur de dégâts"));
        tabWidget->addTab(randomGeneratorScroll, tr("Générateur de fiches aléatoires"));
    }
    else
    {
        tabWidget->addTab(generator, tr("Générateur de fiche"));
        tabWidget->addTab(simulator, tr("Simulateur de dés"));
        tabWidget->addTab(damageCalculator, tr("Calculateur de dégâts"));
        tabWidget->addTab(randomGenerator, tr("Générateur de fiches aléatoires"));
    }
    //We do this to have a little margin to the left & right (prettier)
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    setWindowTitle("Outils pour Kustom");
}
CMainWindow::~CMainWindow()
{
}

void CMainWindow::closeEvent( QCloseEvent *evt )
{
  Q_UNUSED( evt );

  foreach (QWidget *widget, QApplication::topLevelWidgets()) {
    widget->close();
   }
}
