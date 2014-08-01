using UnityEngine;
using System.Collections;
using System;
using System.Xml.Serialization;

public class PathType : ICloneable {
	public string Name;
	public int PathTypeId;
	
	public string Tex;

	//[XmlIgnore]
	//public Material Mat;
	[XmlIgnore]
	public Texture LoadedTex;
	
	// Defined function to return a clone of the current class
	public object  Clone () {
		return this.MemberwiseClone ();
	}
}