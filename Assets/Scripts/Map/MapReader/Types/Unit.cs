using UnityEngine;
using System.Collections;
using System.Xml.Serialization;

public class Unit : UnitType {
	[XmlAttribute]
	public int UnitId;

	[XmlAttribute]
	public int UnitTypeId;

	[XmlAttribute]
	public float PosX;
	[XmlAttribute]
	public float PosY;
	[XmlAttribute]
	public float PosZ;
}