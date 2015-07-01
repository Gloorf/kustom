#!/usr/bin/python3
# -*- coding: utf8 -*-
from lxml import etree, objectify
import zipfile
from xml.dom import minidom
from collections import OrderedDict
from skill import *
skill_type = {10:"w", 11:"m", 12:"d"}
skill_speed = {"Rapide.":"f", "Moyen.":"m", "Lent.":"s"}
kustom_id_type = {"race": "01", "skill":"02", "perk":"03", "misc":"99"}            
def get_id(type_id, int_id):
    output = kustom_id_type[type_id]
    output = output.ljust(6-len(str(int_id)),'0') + str(int_id)
    return output

with zipfile.ZipFile('../data/compétences.odt') as myzip:
    with myzip.open('content.xml') as myfile:
        tree = etree.parse(myfile)
root = tree.getroot()
body = root[3][0]

ns = {'text': 'urn:oasis:names:tc:opendocument:xmlns:text:1.0',
             'table': 'urn:oasis:names:tc:opendocument:xmlns:table:1.0'}
#We check for which skill we are switching the type (to use later)
skill_type_change = []
mega_titles = body.findall('text:h[@text:style-name="Mega_5f_titre"]', ns)
for title in mega_titles:
    current = title.getnext()
    while current.tag != 'h' and current.get("{urn:oasis:names:tc:opendocument:xmlns:text:1.0}style-name") != "Titre_5f_comp":
        current = current.getnext()
    for child in current:
        if child.tail != None:
            skill_type_change.append(child.tail)
titles = body.findall('text:h[@text:style-name="Titre_5f_comp"]', ns)
#We create the skills with correct speed / type
ind_type = 9
skill_counter = 0
for title in titles:
    name = ""
    for child in title:
        if child.tail != None:
            name = child.tail
    if name in skill_type_change:
        ind_type += 1
    #The speed should be directly in the next (which is a <p>), but sometime there is a random child (ty openoffice)
    ind_speed = title.getnext().text
    if ind_speed == None:
        next = title.getnext()
        for child in next:
            if child.tail != None:
                ind_speed = child.tail
    skill = Skill()
    #remove the " :"
    skill.name = name[:-2]
    skill.type = skill_type[ind_type]
    skill.speed = skill_speed[ind_speed]
    skill.id = get_id('skill', skill_counter)
    skills.append(skill)
    skill_counter +=1

forbidden_tables = ["Coût_compétence", "Coût_compétence_cumulé", "Compétence_vrac"]
shitty_skills = ['Armes Doubles et Doubles Armes']
full_name="{urn:oasis:names:tc:opendocument:xmlns:table:1.0}name"
tables = body.findall('table:table', ns)
perk_counter = -1
for table in tables:
    if table.get(full_name) not in forbidden_tables:
        #We found the good table
        name = table.get(full_name).replace("_"," ")
        index,active = find_skill_by_name(skills, name)
        rows = table.findall('table:table-row', ns)
        #These skills are horrible to parse, i'll see later
        if active.name in shitty_skills:
            continue
        rows.pop(0)#Remove the first row because it's the headers
        for row in rows:
            perk_counter += 1
            #We set everything to default value and adjust it based on the skill type
            perk_attrib = OrderedDict([('id', ''), ('name', ''), ('cost', 0), ('level',0),('desc',''),('misc',''),('cooldown',''),('dd',''),('slowIncant',False),('fastIncant',False),('focus',False),('condition',''),('school',''),('optionalComponent',''),('incantTime','')])
            prerequisite = []
            ####MISC####
            if active.is_misc():
                p = row[0].find('text:p', ns).text
                perk_attrib['level'] = int(p.replace(active.name,'').strip())
                cells = row.findall('table:table-cell', ns)
                #remove the first 1, which is the level cell
                cells.pop(0)
                for cell in cells:
                    full_text = ''.join(cell.find('text:p',ns).itertext())
                    if ': ' in full_text:
                        name = full_text.split(': ')[0]
                        #We want to avoid multi-split if there is  a ":" in the desc
                        perk_attrib['desc'] = full_text[len(name):]
                        perk_attrib['name'] = name.rstrip()
                        perk_counter += 1
                        perk_attrib['id'] = get_id('perk', perk_counter)
                        perk = Perk(perk_attrib, prerequisite)
                        skills[index].perks.append(perk)
                        
            #####MAGIC#####          
            #Due to the multiple number of table in magic, i do it cell by cell, doing special cells only for some skills (like the school for skills with multiples schools)  
            elif active.is_magic():
                p = row[0].findall('text:p', ns)
                cost = int(''.join(p[1].itertext()).replace('pts','').rstrip())
                #In case of page break or child inside the <p>
                perk_attrib['name'] = ''.join(p[0].itertext()).rstrip()
                cell_ind = 1
                if active.multiple_schools():
                    perk_attrib['school'] = row[cell_ind].find('text:p', ns).text
                    cell_ind +=1
                perk_attrib['dd'] = row[cell_ind].find('text:p', ns).text
                cell_ind += 1
                if active.id == '020021':#Sorcellerie
                    perk_attrib['optionalComponent'] = ''.join(row[cell_ind].find('text:p',ns).itertext())
                    cell_ind += 1
                if active.id == '020020':#sceau incant time
                    perk_attrib['incantTime'] = ''.join(row[cell_ind].find('text:p',ns).itertext())
                    cell_ind += 1
                if active.id == '020017':#condition for moniale
                    perk_attrib['condition'] = ''.join(row[cell_ind].find('text:p',ns).itertext())
                    cell_ind += 1
                #description can be multilines, so we need to get everything (not keeping \n atm because it looks bad on the thing)
                for child in row[cell_ind].findall('text:p',ns):
                    perk_attrib['desc'] += ''.join(child.itertext())
                cell_ind += 1
                if active.id != '020020':#sceaux no cooldown
                    perk_attrib['cooldown'] = ''.join(row[cell_ind].find('text:p',ns).itertext())
                    cell_ind += 1
                misc = ''.join(row[cell_ind].find('text:p',ns).itertext())
                if 'Incantation instantanée.' in misc:
                    perk_attrib['fastIncant'] = True
                    misc.replace('Incantation instantanée.','')
                if 'Incantation lente.' in misc:
                    perk_attrib['slowIncant'] = True
                    misc.replace('Incantation lente.', '')
                if 'Jet de concentration requis à chaque tour.' in misc:
                    perk_attrib['focus'] = True
                    misc.replace('Jet de concentration requis à chaque tour.','')
                perk_attrib['id'] = get_id('perk', perk_counter)
                
                perk = Perk(perk_attrib, prerequisite)
                skills[index].perks.append(perk)                
            ####WEAPON####
            elif active.is_weapon():
                p = row[0].findall('text:p', ns)
                perk_attrib['name'] = ''.join(p[0].itertext()).rstrip()
                #Sometimes openoffice adds some child (<soft-page-break/> which fucks up this, so we get all the text in the second <p> child of cell
                perk_attrib['cost'] = int(''.join(p[1].itertext()).replace('pts','').rstrip())
                req = row[1].findall('text:p', ns)
                perk_attrib['level'] = int(''.join(req[0].itertext()).replace(skills[index].name,'').strip())
                #multiple p > prerequisite
                prerequisite = []
                if len(req)>1:
                    text = ''.join(req[1].itertext())
                    if 'ou ' in text:
                        multiple_skill = True
                    else:
                        multiple_skill = False
                    is_skill = text[-1:].isdigit()
                    if is_skill:
                        req_lvl = int(text[-1:])
                        #Remove the last character (it's the lvl) + all right whitespace (fckthem)
                        text = text[:-1].rstrip()
                        if multiple_skill:
                            both = text.split('ou ')
                            skill_1 = both[0].rstrip()
                            id_1 = find_skill_by_name(skills,skill_1)[1].id
                            skill_2 = both[1]
                            id_2 = find_skill_by_name(skills,skill_2)[1].id
                            prerequisite = [id_1, id_2, req_lvl]
                        else:
                            skill_1 = text
                            if skill_1 == "Arme":#Any kind of weapon, fck you andreas
                                id_1 = "990001"
                            else:
                                id_1 = find_skill_by_name(skills, skill_1)[1].id
                            prerequisite = [id_1, req_lvl]
                    elif text !="": #we don't want all the case where there is nothing and openoffice derped
                        id=find_perk_by_name(skills[index].perks, text)[1].id
                        prerequisite = [id]
                        
                perk_attrib['desc'] = ''.join(row[2].find('text:p', ns).itertext())
                perk_attrib['cooldown'] = row[3].find('text:p', ns).text
                perk_attrib['misc'] = row[4].find('text:p', ns).text
            #End of parsing
                perk_attrib['id'] = get_id('perk', perk_counter)
                perk = Perk(perk_attrib, prerequisite)
                skills[index].perks.append(perk)
xml_base = '''
    <skills>
    </skills>
    '''          
root = objectify.fromstring(xml_base)
for skill in skills:
    root.append(skill.to_xml())
objectify.deannotate(root)
etree.cleanup_namespaces(root)

obj_xml = etree.tostring(root, encoding='utf-8',pretty_print=True)
reparsed = minidom.parseString(obj_xml).toprettyxml()
with open("../data/skill.xml", "w") as xml_writer:
    xml_writer.write(reparsed)

