using UnityEngine;
using System.Collections;
using System;

public class BuildingType : ICloneable {
	public int BuildingTypeId;
	public string Name;
	public string Model;
	public int BasePath;
	
	// Defined function to return a clone of the current class
	public object  Clone () {
		return this.MemberwiseClone ();
	}
}
