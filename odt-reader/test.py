#!/usr/bin/python3
# -*- coding: utf8 -*-
from lxml import etree, objectify
from xml.dom import minidom
#----------------------------------------------------------------------
def create_appt(data):
    """
    Create an appointment XML element
    """
    appt = objectify.Element("appointment")
    appt.begin = data["begin"]
    appt.uid = data["uid"]
    appt.alarmTime = data["alarmTime"]
    appt.state = data["state"]
    appt.location = data["location"]
    appt.duration = data["duration"]
    appt.subject = data["subject"]
    return appt
 
#----------------------------------------------------------------------
def create_xml():
    """
    Create an XML file
    """
 
    xml = '''
    <zAppointments>
    </zAppointments>
    '''
 
    root = objectify.fromstring(xml)
    root.set("reminder", "15")
 
    appt = create_appt({"begin":1181251680,
                        "uid":"040000008200E000",
                        "alarmTime":1181572063,
                        "state":"",
                        "location":"",
                        "duration":1800,
                        "subject":"Bring pizza home"}
                       )
    root.append(appt)
 
    uid = "604f4792-eb89-478b-a14f-dd34d3cc6c21-1234360800"
    appt = create_appt({"begin":1234360800,
                        "uid":uid,
                        "alarmTime":1181572063,
                        "state":"dismissed",
                        "location":"",
                        "duration":1800,
                        "subject":"Check MS Office website for updates"}
                       )
    root.append(appt)
 
    # remove lxml annotation
    objectify.deannotate(root)
    etree.cleanup_namespaces(root)
 
    # create the xml string
    obj_xml = etree.tostring(root,encoding='utf-8')
    reparsed = minidom.parseString(obj_xml).toprettyxml()
    
    try:
        with open("example.xml", "w") as xml_writer:
            xml_writer.write(reparsed)
    except IOError:
        pass
 
#----------------------------------------------------------------------
if __name__ == "__main__":
    create_xml()
