using UnityEngine;
using System.Collections;
using System.Xml.Serialization;

public class Building : BuildingType {
	[XmlAttribute]
	public int BuildingId;
	
	[XmlAttribute]
	public int BuildingTypeId;

	// TODO should I be using Y or Z?
	[XmlAttribute]
	public int TileX;
	[XmlAttribute]
	public int TileZ;
}
