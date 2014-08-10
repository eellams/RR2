using UnityEngine;
using System.Collections;
using System.Xml.Serialization;

public class Block : TileType {
	[XmlAttribute("TileTypeId")]
	public int TileTypeId;
	
	[XmlAttribute("Height")]
	public float Height;
	
	[XmlAttribute("EC")]
	public int EC;
	
	[XmlAttribute("ORE")]
	public int ORE;
	
	[XmlAttribute("ID")]
	public int ID;
	
	[XmlAttribute("Tex")]
	string Tex;

	[XmlAttribute("PathTypeId")]
	public int PathTypeId;
	
	// TODO Things like corrosion and avalanche and monster
}
