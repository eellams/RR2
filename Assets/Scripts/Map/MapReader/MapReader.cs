using System.Collections;
using System.Collections.Generic;
using System.Xml.Serialization;
using System.IO;
using UnityEngine;
using System;

public class MapReader : MonoBehaviour {
	public XmlMap LoadMap(string mapDir) {
		// TODO get a more efficient map format

		// Seraialise the map
		XmlSerializer serializer = new XmlSerializer (typeof(XmlMap));

		// TODO work out why these NEVER WORK
		serializer.UnknownNode += new XmlNodeEventHandler (serializer_UnknownNode);
		serializer.UnknownAttribute += new XmlAttributeEventHandler (serializer_UnknownAttribute);

		// Open the file
		FileStream fs = new FileStream (mapDir, FileMode.Open);

		// Deserialise the xml file
		XmlMap toReturn = (XmlMap)serializer.Deserialize (fs);

		// Close the file
		fs.Close ();

		return toReturn;
	}

	void serializer_UnknownNode(object sender, XmlNodeEventArgs e)
	{
		Debug.Log("Unknown Node:" + e.Name + "\t" +e .Text);
	}
	
	void serializer_UnknownAttribute(object sender, XmlAttributeEventArgs attr)
	{
		Debug.Log("Unknown Attribute:" + attr.Attr.Name + "='" + attr.Attr.Value + "'");
	}
}