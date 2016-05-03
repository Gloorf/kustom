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
#include "data.h"
#include "data_type.h"
#include "odt_writer.h"
#include "generator.h"
#include <QTextDocumentWriter>
COdtWriter::COdtWriter(CCharacter *character, QFont font, CData *data)
{
    _usedFont=font;
    d=data;
    _c=character;
    QString sheetSavePath=d->getSettings()->value("sheetSavePath").value<QString>();
    QDir dir(sheetSavePath);
    if(!dir.exists())
    {
        dir.current().mkdir(sheetSavePath);
    }
    QString fileName = !_c->getName().isEmpty() ? sheetSavePath + _c->getName() + ".odt" : sheetSavePath + "défaut.odt";
    QFile file(fileName);
    if(file.exists())
    {
        bool ok;
        QString newFileName = QInputDialog::getText(this, tr("Le fichier existe déjà !"),
            tr("Nouveau nom de fichier, sans .odt (laissez vide pour écraser)"), QLineEdit::Normal,
        "",&ok);
        if(!newFileName.isEmpty())
        {
            fileName= sheetSavePath + newFileName+".odt";
        }
        else
        {
            file.remove();
        }
    }
    _document = new QTextDocument();
    _cursor= QTextCursor(_document);
    createStyle();
    writeCharData();
    writeSkillData();
    writeMiscSheet();
    writeFile(fileName);
}

COdtWriter::~COdtWriter()
{

}

void COdtWriter::writeFile(QString &fileName)
{

    QFile file(fileName);
    QTextDocumentWriter writer(fileName);
    writer.setFormat("odf");
    writer.write(_document);
}

void COdtWriter::writeCharData()
{

    QTextBlockFormat centerBlock;
    QTextBlockFormat regularBlock;
    centerBlock.setAlignment(Qt::AlignCenter);
    _cursor.setBlockFormat(centerBlock);
    _cursor.insertText(_c->getName()+ " - " + _c->getRaceName(), _title1);
    _cursor.insertBlock();
    _cursor.insertText(QString::number(_c->getPointsTotal()) + "K", _title1);//Kustom point cost

    _cursor.insertBlock();
    _cursor.setBlockFormat(regularBlock);

    _cursor.insertText("Caractéristiques et réduction :", _title2);
    _cursor.insertBlock();
    _cursor.insertBlock();
    QTextTableFormat caracTable;
    caracTable.setBorderStyle(QTextFrameFormat::BorderStyle_None);
    _cursor.insertTable(6, 2, caracTable);
    _cursor.insertText("Pvs : " + QString::number(_c->getAttribute("health")), _italic);

    if(_c->getAttribute("healthReligion") > 0)
    {
        _cursor.insertText("(" + QString::number(_c->getAttribute("health")+_c->getAttribute("healthReligion") ) + ")", _italic);
    }


    _cursor.movePosition(QTextCursor::Down);
    _cursor.insertText("Force : " + QString::number(_c->getAttribute("strength")), _italic);
    int strBonus = _c->getAttribute("strength")/4;
    if(_c->getAttribute("strengthReligion") > 0)
    {
        _cursor.insertText("(" + QString::number(_c->getAttribute("strength")+_c->getAttribute("strengthReligion") ) + ")", _italic);
        strBonus += _c->getAttribute("strengthReligion")/4;
    }
    _cursor.insertText(+ " (+" + QString::number(strBonus) + ")", _italic);
    _cursor.movePosition(QTextCursor::Down);
    _cursor.insertText("Agilité : " + QString::number(_c->getAttribute("agility")) + " (+" + QString::number(_c->getAttribute("agility")/3) + ")", _italic);
    _cursor.movePosition(QTextCursor::Down);
    _cursor.insertText("Réflexes : " + QString::number(_c->getAttribute("reflexe")) + " (+" + QString::number(_c->getAttribute("reflexe")/3) + ")", _italic);
    _cursor.movePosition(QTextCursor::Down);
    _cursor.insertText("Volonté : " + QString::number(_c->getAttribute("willpower")) + " (+" + QString::number(_c->getAttribute("willpower")/2) + ")", _italic);
    _cursor.movePosition(QTextCursor::Down);
    _cursor.insertText("Mana : " + QString::number(_c->getAttribute("mana")), _italic);
    _cursor.movePosition(QTextCursor::NextCell);
    _cursor.insertText("Pieds et jambes : ", _italic);
    _cursor.movePosition(QTextCursor::Up);
    _cursor.insertText("Mains : ", _italic);
    _cursor.movePosition(QTextCursor::Up);
    _cursor.insertText("Bras :", _italic);
    _cursor.movePosition(QTextCursor::Up);
    _cursor.insertText("Torse : ", _italic);
    _cursor.movePosition(QTextCursor::Up);
    _cursor.insertText("Buste : ", _italic);
    _cursor.movePosition(QTextCursor::Up);
    _cursor.insertText("Tête : ", _italic);
    _cursor.movePosition(QTextCursor::End);
    _cursor.insertBlock();
    _cursor.insertBlock();

}

void COdtWriter::writeSkillData()
{
    _cursor.insertText("Compétences :", _title2);
    _cursor.insertBlock();
    _cursor.insertBlock();

    for(int i=0;i<_c->getSkill().size();i++)
    {
        CUSkill *skill = _c->getSkill()[i];
        QString tmpName =  skill->getParam("name") + " " + skill->getParam("level");
        //Need to move that
        if(skill->multipleSchool() && skill->getParam("special")=="t")
        {
            tmpName = skill->getParam("special")=="t" ? tmpName + " (talent : "+ skill->getSchool() +")" : tmpName;
        }
        else
        {
            tmpName = skill->getParam("special")=="t" ? tmpName + " (talent)" : tmpName;
        }
        tmpName = skill->getParam("special")=="g" ? tmpName + " (don)" : tmpName;
        tmpName = tmpName + " : ";
        _cursor.insertText(tmpName, _italic);
        //We want to have some space between the member of the list, so we add the margin (need to be done every time :()
        QTextBlockFormat regularBlock;
        regularBlock.setBottomMargin(8);
        _cursor.setBlockFormat(regularBlock);
        _cursor.insertList(_skillStyle);


        for(int j=0;j<skill->getPerkB().size();j++)
        {
            _cursor.insertText("  ",_regular);
            if(skill->getPerkB()[j]->getParamStr("school").isEmpty())
            {
                _cursor.insertText(skill->getPerkB()[j]->getParamStr("name") + " :", _underline);
            }
            else
            {
                _cursor.insertText(skill->getPerkB()[j]->getParamStr("name") + " ("+ skill->getPerkB()[j]->getParamStr("school") +") :", _underline);
            }
            _cursor.insertText(" ", _regular);
            insertDesc(skill->getPerkB()[j], skill);
            if(j+1==skill->getPerkB().size())//don't add another block if we are at the last skill !
            {
                break;
            }
            _cursor.insertBlock();
        }
        _cursor.insertBlock();
        if(skill->isWeapon())
        {
            _cursor.insertText("  +X au toucher/esquive distance, +Y à l'esquive CàC", _regular);
        }
        else if(skill->isMagic())
        {
            _cursor.insertText("  +X à l'école, +Y au cast", _regular);
        }
        //Close the List
        QTextCharFormat textCharFormat = _cursor.blockCharFormat();
        QTextBlockFormat blockFormat;
        _cursor.insertBlock(blockFormat,textCharFormat);


    }


}

void COdtWriter::writeMiscSheet()
{
    _cursor.insertBlock();
    _cursor.insertBlock();
    QTextCharFormat regular2 = _regular;
    regular2.setForeground(QColor("red"));
    _cursor.insertText("DD", regular2);
    _cursor.insertText("/", _regular);
    regular2.setForeground(QColor("darkolivegreen"));
    _cursor.insertText("durée", regular2);
    _cursor.insertText("/", _regular);
    regular2.setForeground(QColor("purple"));
    _cursor.insertText("CD", regular2);
    regular2.setForeground(QColor("blue"));
    _cursor.insertText("/",_regular);
    _cursor.insertText("coût en mana", regular2);
    _cursor.insertText("/", _regular);
    regular2.setForeground(QColor("orange"));
    _cursor.insertText("IL+II+concentration", regular2);
    _cursor.insertText("/", _regular);
    _cursor.insertText("description", _regular);
    _cursor.insertBlock();
    regular2.setForeground(QColor("darkolivegreen"));
    _cursor.insertText("durée", regular2);
    _cursor.insertText("/", _regular);
    regular2.setForeground(QColor("purple"));
    _cursor.insertText("CD", regular2);
    _cursor.insertText("/", _regular);
    _cursor.insertText("description", _regular);
    _cursor.insertBlock();
    _cursor.insertBlock();
    _cursor.insertText("Avantages et défauts :", _title2);
    _cursor.insertBlock();
    _cursor.insertBlock();
    for(int i=0;i<_c->getReligionPowerList().size();i++)
    {
        religionPower power = _c->getReligionPowerList()[i];
        _cursor.insertText(power.name + " (" + QString::number(power.cost) + " points)", _regular);
        _cursor.insertBlock();
        _cursor.insertBlock();
    }

    _cursor.insertBlock();
    _cursor.insertText("Inventaire :", _title2);
    _cursor.insertBlock();
    _cursor.insertBlock();
}
/*DD/durée/CD/coût en mana/IL+concentr/description -> magic
durée/CD/description -> weapon
description -> misc
 * */

void COdtWriter::insertDesc(CPerk *perk, CSkill *skill)
{
    QTextCharFormat regular2 = _regular;
    if(skill->isWeapon())//Weapon
    {
        regular2.setForeground(QColor("darkolivegreen"));
        _cursor.insertText("??", regular2);
        _cursor.insertText("/", _regular);
        QString length = perk->getParamStr("cooldown").isEmpty() ? "-" : perk->getParamStr("cooldown");
        regular2.setForeground(QColor("purple"));
        _cursor.insertText(length, regular2);
        _cursor.insertText("/", _regular);
        _cursor.insertText(perk->getParamStr("desc"), _regular);
        _cursor.insertText(" " + perk->getParamStr("misc"), _regular);
    }
    else if(skill->isMagic())//Magic
    {
        regular2.setForeground(QColor("red"));
        _cursor.insertText(perk->getParamStr("dd"), regular2);
        _cursor.insertText("/", _regular);
        regular2.setForeground(QColor("darkolivegreen"));
        _cursor.insertText("??", regular2);
        _cursor.insertText("/", _regular);
        QString length = perk->getParamStr("cooldown").isEmpty() ? "-" : perk->getParamStr("cooldown");
        regular2.setForeground(QColor("purple"));
        _cursor.insertText(length, regular2);
        regular2.setForeground(QColor("blue"));
        _cursor.insertText("/",_regular);
        _cursor.insertText("??", regular2);
        _cursor.insertText("/", _regular);
        QString specialMagic = perk->getParamBool("slowIncant") ? "IL" : "";
        specialMagic = perk->getParamBool("fastIncant") ? specialMagic + "+II" : specialMagic;
        specialMagic = perk->getParamBool("focus") ? specialMagic + "+concentr" : specialMagic;
        if(specialMagic.indexOf("+")==0)
        {
            specialMagic.remove(0,1);
        }
        specialMagic = specialMagic.isEmpty() ? "-" : specialMagic;
        regular2.setForeground(QColor("orange"));
        _cursor.insertText(specialMagic, regular2);
        _cursor.insertText("/", _regular);
        _cursor.insertText(perk->getParamStr("desc"), _regular);
        _cursor.insertText(" " + perk->getParamStr("misc"), _regular);
        if(skill->getParam("name")=="Sorcellerie")
        {
            regular2.setForeground(QColor("red"));
            _cursor.insertText(" " + perk->getParamStr("optionalComponent"), regular2);
        }
        if(skill->getParam("name")=="Sceaux")
        {
            regular2.setForeground(QColor("red"));
            _cursor.insertText(" " + perk->getParamStr("incantTime"), regular2);
        }
    }
    else if(skill->isMisc())//Misc
    {
        _cursor.insertText(perk->getParamStr("desc"), _regular);
    }
}

void COdtWriter::createStyle()
{
    //usedFont= QFont("Linux Libertine Display O", 12);
    _regular.setFont(_usedFont);
    _italic.setFont(_usedFont);
    _underline.setFont(_usedFont);
    _title1.setFont(_usedFont);
    _title2.setFont(_usedFont);

    _italic.setFontItalic(true);
    _underline.setFontUnderline(true);

    _title1.setFontPointSize(16);
    _title1.setFontWeight(QFont::Bold);
    _title1.setFontUnderline(true);
    _title2.setFontPointSize(14);
    _title2.setFontWeight(QFont::Bold);
    _title2.setFontUnderline(true);

    _skillStyle.setStyle(QTextListFormat::ListCircle);


}
