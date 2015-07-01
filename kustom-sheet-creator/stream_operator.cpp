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
#include "data.h"

/*
 * operator for loading / saving to file
*/
QDataStream &operator<<( QDataStream &stream, const skillData &data )
{
  stream << data.name;
  stream << data.speed;
  stream << data.type;
  stream << data.special;
  stream << data.level;
  stream << data.school;

  return stream;
}
QDataStream &operator>>( QDataStream &stream, skillData &data )
{
  stream >> data.name;
  stream >> data.speed;
  stream >> data.type;
  stream >> data.special;
  stream >> data.level;
  stream >> data.school;
  return stream;
}

QDataStream &operator<<( QDataStream &stream, const perkData &data)
{
    stream << data.name;
    stream << data.cost;
    stream << data.level;
    stream << data.desc;
    stream << data.misc;
    stream << data.cooldown;
    stream << data.levelNeeded;
    stream << data.dd;
    stream << data.slowIncant;
    stream << data.fastIncant;
    stream << data.focus;
    stream << data.condition;
    stream << data.school;
    stream << data.optionalComponent;
    stream << data.incantTime;
    return stream;
}

QDataStream &operator>>( QDataStream &stream,perkData &data)
{
    stream >> data.name;
    stream >> data.cost;
    stream >> data.level;
    stream >> data.desc;
    stream >> data.misc;
    stream >> data.cooldown;
    stream >> data.levelNeeded;
    stream >> data.dd;
    stream >> data.slowIncant;
    stream >> data.fastIncant;
    stream >> data.focus;
    stream >> data.condition;
    stream >> data.school;
    stream >> data.optionalComponent;
    stream >> data.incantTime;
    return stream;
}
QDataStream &operator<<( QDataStream &stream, const CSkill &data)
{
    stream << data.getInfo();
    stream << data.getPerkB();
    return stream;
}
QDataStream &operator>>( QDataStream &stream, CSkill &data)
{
    skillData info;
    QList<perkData> perkB;

    stream >> info >> perkB;
    data.setInfo(info);
    data.setPerkB(perkB);
    return stream;
}

QDataStream &operator<<( QDataStream &stream, const damageData &data)
{
    stream << data.value;
    stream << data.roll;
    stream << data.bonus;
    stream << data.max;
    return stream;
}

QDataStream &operator>>( QDataStream &stream, damageData &data)
{
    stream >> data.value;
    stream >> data.roll;
    stream >> data.bonus;
    stream >> data.max;
    return stream;
}

QDataStream &operator>>( QDataStream &stream, religionPower &data)
{
    stream >> data.name;
    stream >> data.cult;
    stream >> data.cost;
    stream >> data.special;
    return stream;
}

QDataStream &operator<< ( QDataStream &stream, const religionPower &data)
{
    stream << data.name;
    stream << data.cult;
    stream << data.cost;
    stream << data.special;
    return stream;
}
