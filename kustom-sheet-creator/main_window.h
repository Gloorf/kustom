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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtGui>
#include <QApplication>
#include <QDebug>
#include <QtWidgets>
#include "generator.h"
#include "simulator.h"
#include "damage_calculator.h"
#include "data.h"
#include "random_generator.h"
class CMainWindow : public QMainWindow
{
Q_OBJECT
public:
    CMainWindow();
    ~CMainWindow();




private:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QVBoxLayout *mainLayout;
    CData *data;
    CGenerator *generator;
    QTabWidget *generatorTab;
    CSimulator *simulator;
    CDamageCalculator *damageCalculator;
    CRandomGenerator *randomGenerator;
    CVerticalScrollArea *generatorScroll;
    CVerticalScrollArea *simulatorScroll;
    CVerticalScrollArea *damageCalculatorScroll;
    CVerticalScrollArea *randomGeneratorScroll;
    void closeEvent( QCloseEvent *evt);
private slots:
    void onGeneratorTabChange(int index);
    void onGeneratorTabClose(int index);
    void onRandomSheetGenerated(CCharacter *character);
};





#endif
