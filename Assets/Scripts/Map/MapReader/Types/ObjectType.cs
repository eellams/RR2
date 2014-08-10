using UnityEngine;
using System.Collections;
using System;

public class ObjectType : ICloneable {
	public string Name;
	public int ObjectTypeId;
	public int ORE;
	public int EC;
	public string Model;

	// Defined function to return a clone of the current class
	public object  Clone () {
		return this.MemberwiseClone ();
	}
}
