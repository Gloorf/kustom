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

#include "util.h"
#include <QtWidgets>
#include <random>
std::default_random_engine generator;
qint32 Util::randInt(qint32 low, qint32 high)
{
    std::uniform_int_distribution<int> distribution(low,high);

    return distribution(generator);  // generates number in the range 1..6
}
