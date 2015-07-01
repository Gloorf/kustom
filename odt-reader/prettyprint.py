#!/usr/bin/python3
# -*- coding: utf8 -*-
import zipfile

import xml.dom.minidom
with zipfile.ZipFile('../data/compétences.odt') as myzip:
    with myzip.open('content.xml') as myfile:
        xml = xml.dom.minidom.parse(myfile)

pretty_xml = xml.toprettyxml()
with open('pretty_content.xml', 'w') as file:
    file.write(pretty_xml)
