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

#ifndef XML_READER_H
#define XML_READER_H
#include <QApplication>
#include <QtCore>
#include <QXmlStreamReader>
#include <QtGui>
#include <QtWidgets>
#include "data_type.h"
class CXmlReader : public QObject
{
    Q_OBJECT

public:
    CXmlReader();
    QList<CReligion *> loadReligion();
    QList<CRace *> loadRace();
    QList<QList<QList<damageData> > > loadDamage();
    QList<CSkill *> loadSkill();
    QStringList bodyPartName;
    QList<QStringList> bodyZoneName;

};

#endif // XML_READER_H
