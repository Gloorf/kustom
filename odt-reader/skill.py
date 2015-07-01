#!/usr/bin/python3
# -*- coding: utf8 -*-
from lxml import etree, objectify
import zipfile
from xml.dom import minidom
from collections import OrderedDict
class Perk(object):
    def __init__(self,initial_data, prerequisite):
        super(Perk, self).__setattr__('_odict', OrderedDict())
        for key,value in initial_data.items():
            setattr(self, key, value)
        #array of either 1,2 or 3 value : 1 is perk (id), 2 is 1 skill (id+level), 3 is 1/2 skill (id1 + id2 + level)
        self.prerequisite=prerequisite
    def __getattr__(self, key):
        odict = super(Perk, self).__getattribute__('_odict')
        if key in odict:
            return odict[key]
        return super(Perk, self).__getattribute__(key)

    def __setattr__(self, key, val):
        self._odict[key] = val

    @property
    def __dict__(self):
        return self._odict
    def print(self):
        print("  id : {0}\n   name : {1}\n   cost: {2}\n   level: {3}\n   desc: {4}".format(self.id, self.name, self.cost, self.level, self.desc))
    def to_xml(self, parent):
        pk = objectify.SubElement(parent, "perk")
        for key, value in self.__dict__.items():
            setattr(pk, key, value)
        if len(self.prerequisite) == 0:
            pre = objectify.SubElement(pk, "prerequisite", {'type':"none"})
        elif len(self.prerequisite) == 1:#One perk
            pre = objectify.SubElement(pk, "prerequisite", {'type':"perk"})
            pre.id = self.prerequisite[0]
        elif len(self.prerequisite) == 2:#One skill
            pre = objectify.SubElement(pk, "prerequisite", {'type':"skill"})
            pre.id = self.prerequisite[0]
            pre.level = self.prerequisite[1]
        elif len(self.prerequisite) == 3:
            pre = objectify.SubElement(pk, "prerequisite", {'type':"skill", 'number':'2'})
            pre.id1 = self.prerequisite[0]
            pre.id2 = self.prerequisite[1]
            pre.level = self.prerequisite[2]
        return pk

                        
     
class Skill(object):
    def __init__(self):
        self.id="990000"
        self.name=""
        self.speed=""
        self.type=""
        self.perks=[]
    def print(self):
        print("SKILL")
        print(" id : {0}\n name : {1} \n speed : {2} \n type : {3}".format(self.id, self.name, self.speed, self.type))
        print(" perks :")
        for perk in self.perks:
            perk.print()
    def to_xml(self):
        sk = objectify.Element("skill")
        sk.id = self.id
        sk.name = self.name
        sk.speed = self.speed
        sk.type = self.type
        sk.perks = objectify.SubElement(sk, 'perks')
        for perk in self.perks:
            truc = perk.to_xml(sk.perks)
        return sk
    def is_weapon(self):
        if self.type == 'w':
            return True
        if self.id == '020027' or self.id =='020040': #Défense & Lutte
            return True
        return False
    def is_magic(self):
        if self.type == 'm':
            return True
        return False
    def is_misc(self):
        return not self.is_weapon() and not self.is_magic()
    def multiple_schools(self):
        if self.id == '020024' or self.id == '020022' or self.id == '020017' or self.id == '020019':#Psychique/Ele/moniale/occultisme
            return True
        return False

def find_skill_by_name(skills, name):
    for index,skill in enumerate(skills):
        if skill.name == name:
            return index,skill
    default=-1
    return default,Skill()
            
def find_perk_by_name(perks, name):
    for index, perk in enumerate(perks):
        if perk.name == name:
            return index,perk
    default=-1
    return default,Perk({}, [])
