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

#ifndef RANDOM_H
#define RANDOM_H
#include "data.h"

class CRandom
{
public:
    CRandom(CData *data);
    CRace* randomRace();
    CSkill* randomSkill(qint32 min_lvl = 1, qint32 max_lvl = 10);
private:
    CData *_d;
};

#endif // RANDOM_H

