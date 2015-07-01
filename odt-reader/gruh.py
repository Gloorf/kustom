#!/usr/bin/python3
# -*- coding: utf8 -*-
from lxml import etree, objectify
from xml.dom import minidom
class Perk(object):
    def __init__(self, name, id):
        self.name = name
        self.id = id
    def to_xml(self, parent):
        pk = objectify.SubElement(parent, 'perk')
        for key,val in self.__dict__.items():
            setattr(pk, key, val)

        
        return pk
class Skill(object):
    def __init__(self, initial_data, perks):
        for key in initial_data:
            setattr(self, key, initial_data[key])
        self.perks = perks

    def to_xml(self):
        sk = objectify.Element("skill")
        sk.name = self.name
        sk.id = self.id
        sk.perks = objectify.SubElement(sk, 'perks')
        for perk in self.perks:
            truc = perk.to_xml(sk.perks)
        return sk
        
perks = [Perk("Perk 1", "31"), Perk("Perk 2", "32")]
attr = {"name": "Skill", "id": "01"}
skill = Skill(attr, perks)
val = perks[0].__dict__
for k,v in val.items():
    print(k,v)
xml = '''
    <skills>
    </skills>'''
root = objectify.fromstring(xml)
root.append(skill.to_xml())
#On enlève les annotations + les namespace pour avoir un truc simple
objectify.deannotate(root)
etree.cleanup_namespaces(root)
obj_xml = etree.tostring(root, encoding='utf-8')
#Le pretty print de etree marche pas, on prend celui de minidom à la place 
reparsed = minidom.parseString(obj_xml).toprettyxml()
with open("output.xml", "w") as xml_writer:
    xml_writer.write(reparsed)
