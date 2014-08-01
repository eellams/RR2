using UnityEngine;
using System.Collections;
using System.Xml.Serialization;

[XmlRoot("Map")]
public class XmlMap {
	public string Name;
	public int Width;
	public int Height;

	[XmlArrayAttribute("Blocks")]
	public Block[] Blocks;
	
	[XmlArrayAttribute("TileTypes")]
	public TileType[] TileTypes;
	
	[XmlArrayAttribute("UnitTypes")]
	public UnitType[] UnitTypes;

	[XmlArrayAttribute("PathTypes")]
	public PathType[] PathTypes;
	
	[XmlArrayAttribute("Units")]
	public Unit[] Units;
}