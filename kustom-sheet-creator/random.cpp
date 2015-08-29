/*
Copyright (C) 2015 Guillaume DUPUY <glorf@glorf.fr>
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
#include "random.h"
#include "util.h"
CRandom::CRandom(CData *data) : _d(data)
{

}

CRace* CRandom::randomRace()
{
    QString id = _d->getFullId("race", Util::randInt(0, _d->getRaceB().size() -1));
    return _d->getRaceById(id);
}

CSkill* CRandom::randomSkill(qint32 min_lvl, qint32 max_lvl)
{
    QString id = _d->getFullId("skill", Util::randInt(0, _d->getSkillB().size() -1));
    CSkill *sk = _d->getSkillById(id);
    sk->setParam("level", QString::number(Util::randInt(min_lvl, max_lvl)));
    QStringList spec;
    spec << "n" << "t" << "g";
    sk->setParam("special", spec[Util::randInt(0,2)]);
    return sk;
}
