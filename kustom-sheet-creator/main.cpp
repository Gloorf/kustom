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
#include "xml_reader.h"
#include "character.h"
int main(int argc, char * argv[])
{
  //QTextCodec::setCodecForTr(       QTextCodec::codecForName("UTF-8") );
  //QTextCodec::setCodecForCStrings( QTextCodec::codecForName("UTF-8") );
    QApplication app(argc, argv);

    CMainWindow mainWindow;
    mainWindow.show();
    return app.exec();
    //Test
}
/*
DONE-ish Obviously pouvoirs de religion
RàZ fiche
DONE demander l'école d'un talent magique à multi école
DONE (mais en python) refaire marcher le lecteur de .odt


 * BUG :
 * DONE Acrobaties & athlétisme : deux fois la même compétence
 * DONE crash info comp si pas de comp perso
 * DONE crash scroll armes doubles
 * DONE ct tout moche quand on changeait de skill dans la fenêtre d'info
 * TODO
 * Général :
 * DONE Déplacer les infos d'un perso dans une classe (ie pas tout stocker comme un malpropre dans CGenerator)
 * DONE Transformer les .dat binaire en .xml (et changer la lecture obviously)
 * Tooltips partout !
 * DONE Mana inshum buggué
 * Modifier les options du kustom.ini en live
 * Calculateur :
 * DONE À virer
 * Générateur :
 * DONE Prévenir si on supprime un fichier
 * Générer une fiche aléatoire
 * DONE Choisir lieu de sauvegarde des fichiers
 * DONE Une option pour réinitialiser la fiche (avec confirmation)
 * Système multi-fiches
 * Lecture de fiche créée par KCS ?
 * Odt-Reader :
 * -Trouver les comps à prix réduit pour certaines races
 * -Essayer de trouver la durée d'un spell / skill
 * -Essayer de trouver le cout en mana
 * DONE MOUAHAHAH -Faire marcher forge technomagique
 * Description de sort & retour à la ligne ? Récupération OK, mais le pb est dans odt_writer (c'est moooche)
 * Odt-writer :
 * -DONE Dans Défense, le texte des perk est écrit due xfois
 */
