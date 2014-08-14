﻿using UnityEngine;
using System.Collections;
using System;
using System.Xml.Serialization;

public class TileType : ICloneable {
	public string Name;
	public int TileTypeId;
	public float Height;
	public bool Solid;
	public float DrillTime;
	public int PathTypeWhenDrilled;
	
	public string Tex;

	[XmlArrayAttribute("Drops")]
	public Drop[] Drops;
	
	[XmlIgnore]
	public UInt32 Surround;
	[XmlIgnore]
	public int ID;
	[XmlIgnore]
	public Material Mat;
	[XmlIgnore]
	public Mesh mesh;
	[XmlIgnore]
	public int PathTypeId;
	
	// Defined function to return a clone of the current class
	public object  Clone () {
		return this.MemberwiseClone ();
	}
}

public class Drop {
	[XmlAttribute]
	public int ObjectTypeId;

	[XmlAttribute]
	public int Min;

	[XmlAttribute]
	public int Max;
}