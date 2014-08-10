using UnityEngine;
using System.Collections;
using System;

public class UnitType : ICloneable {
	public int UnitTypeId;
	public string Name;
	public string Model;
	
	// Defined function to return a clone of the current class
	public object  Clone () {
		return this.MemberwiseClone ();
	}
}
