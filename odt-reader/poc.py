#!/usr/bin/python3
# -*- coding: utf8 -*-
#Problématique : on a un <h> qui a un élement sans texte (<bookmark-start />), qui empêche de récupérer le texte du <h>
from lxml import etree
fragment='<h style-name="Mega_5f_titre"><bookmark-start name="12341"/>ARMES<bookmark-end name="12341"/></h>'
div = etree.fromstring(fragment)
print("texte de mon titre : {0}".format(div[0].tail))
fragment2='<h style-name="Mega_5f_titre">ARMES</h>'
div2 = etree.fromstring(fragment2)
print("texte de mon titre sans enfant : " + div2.text)
#Ça marche bien sans enfants, le problème est bien les élements sans texte !!
#Si on essaie d'enlever les enfants, on a toujours un text = None ...
for child in div:
    div.remove(child)
print("texte après suppression des enfants : {0}".format(div.tail))
