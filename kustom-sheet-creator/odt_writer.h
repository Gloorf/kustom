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
#ifndef ODT_WRITER_H
#define ODT_WRITER_H
#include <QtGui>
#include <QApplication>
#include <QtWidgets>
#include "character.h"
#include "data.h"
#include "data_type.h"

class COdtWriter : public QWidget
{
    Q_OBJECT
public:
    COdtWriter(CCharacter *character, QFont font, CData *data);
    ~COdtWriter();
private:
    CData *d;
    CCharacter *_c;
    QTextDocument *_document;
    QTextCursor _cursor;
    QFont _usedFont;
    QTextCharFormat _title1;
    QTextCharFormat _title2;
    QTextCharFormat _regular;
    QTextCharFormat _italic;
    QTextCharFormat _underline;
    QTextListFormat _skillStyle;


    void writeCharData();
    void writeFile(QString &fileName);
    void createStyle();
    void writeSkillData();
    void writeMiscSheet();
    void insertDesc(CPerk *perk, CSkill *skill);
};

#endif // ODT_WRITER_H
