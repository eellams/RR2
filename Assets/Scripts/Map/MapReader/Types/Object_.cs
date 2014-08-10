using UnityEngine;
using System.Collections;
using System;
using System.Xml.Serialization;

public class Object_ : ICloneable {
	[XmlAttribute("ObjectTypeId")]
	public int ObjectTypeId;

	[XmlAttribute]
	public int ObjectId;

	[XmlAttribute]
	public float PosX;
	[XmlAttribute]
	public float PosY;
	[XmlAttribute]
	public float PosZ;

	// Defined function to return a clone of the current class
	public object  Clone () {
		return this.MemberwiseClone ();
	}
}
