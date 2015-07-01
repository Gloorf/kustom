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
#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <QtGui>
#include <QApplication>
#include <QtWidgets>
class CSimulator : public QWidget
{
    Q_OBJECT
public:
    CSimulator();
    ~CSimulator();

private:
    struct diceData
    {
        qint32 value;//Max of the dice
        qint32 roll;//How many time we roll a dice
        qint32 bonus;//+C
    };
    qint32 diceRoll(diceData dice);
    qint32 randInt(qint32 low, qint32 high);
    void createDiceBox();
    void createProbabilityBox();
    void createSimulatorLayout();
    bool isDice(QString rawText);

    //Graphic stuff
    QGroupBox *diceBox;
    QGroupBox *probabilityBox;

    QGridLayout *diceGrid;
    QGridLayout *probabilityGrid;

    QVBoxLayout *simulatorLayout;
    QPushButton *buttonDiceRoll;
    QPushButton *buttonProbabilityRoll;
    //Input data
    struct diceInput
    {
        QLineEdit *value;
        QSpinBox *result;
    };
    struct probabilityInput
    {
        QLineEdit *value1;
        QLineEdit *value2;
        QDoubleSpinBox *win;
        QDoubleSpinBox *draw;
        QDoubleSpinBox *losse;
    };

    QList<diceInput> diceI;
    QList<probabilityInput> probabilityI;
    QSpinBox *diceNumberInput;
    QSpinBox *probabilityNumberInput;
    //Data stuff (based on user input)
    qint32 diceNumber;//How many dice to display
    qint32 probabilityNumber;
    qint32 probabilityRoll;//How many roll we do to find a probability

    QList<QList<diceData> > diceBoxD;//Double QList because we can have multiple dice for one roll
    /*a dice is AdB+C ; the List is {A;B;C} for each dice (with two dice we'll have
     * {A1;B1;C1;A2;B2;C2} )
     * */
    QList<QList<diceData> > probabilityBoxD1; //Can be static
    QList<QList<diceData> > probabilityBoxD2; //Can be static
private slots:
    void updateDiceValue();
    void updateProbabilityValue();
    void updateDiceNumber();
    void updateProbabilityNumber();
};

#endif // SIMULATOR_H
