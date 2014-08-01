using UnityEngine;
using System.Collections;
using System;
using System.Xml.Serialization;

public class TileType : ICloneable {
	public string Name;
	public int TileTypeId;
	public float Height;
	public int EC;
	public int ORE;
	public bool Solid;
	public float MoveWeight;
	
	public string Tex;
	
	[XmlIgnore]
	public UInt32 Surround;
	[XmlIgnore]
	public int ID;
	[XmlIgnore]
	public Material Mat;
	[XmlIgnore]
	public Mesh mesh;
	
	// Defined function to return a clone of the current class
	public object  Clone () {
		return this.MemberwiseClone ();
	}
}