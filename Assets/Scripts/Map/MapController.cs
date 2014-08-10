using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Collections.Generic;


[RequireComponent (typeof (MapBuilder))]
[RequireComponent (typeof (MapReader))]
[RequireComponent (typeof (MeshFilter))]
[RequireComponent (typeof (MeshRenderer))]
[RequireComponent (typeof (MeshCollider))]
[RequireComponent (typeof (Map))]

public class MapController : MonoBehaviour {
	public Material baseMaterial;

	// Use this for initialization
	void Start () {
		// Get the required components
		MapReader mapReader = GetComponent<MapReader> ();
		Map map = GetComponent<Map> ();
		MapBuilder mapBuilder = GetComponent<MapBuilder> ();

		// Load the map
		XmlMap xmlMap = mapReader.LoadMap (Path.Combine (Environment.CurrentDirectory, "Data/testmap.xml"));

		// This is on a sepeate gameobject, it just seemed more sensible at the time
		GameObject unitGameobject = GameObject.Find ("Units");
		UnitHandler unitHandler = unitGameobject.GetComponent<UnitHandler> ();
		NewAstar aStar = unitGameobject.GetComponent<NewAstar> ();

		// Use required data from xmlMap
		unitHandler.Initialise (xmlMap);

		GameObject worldGameobject = GameObject.Find ("World");
		ObjectController objectController = worldGameobject.GetComponent<ObjectController> ();

		objectController.Initialise (xmlMap);

		// Initialise the map (use required data from xmlMap)
		map.Initialise (baseMaterial, xmlMap);

		// Initialise the A* path finding
		aStar.Initialise ();

		// Build the map
		// Also recalculates A*, which is nice ^_^
		mapBuilder.ProcessMap ();
	}
	
	// Update is called once per frame
	void Update () {
		// TODO avalanche, corrosion etc.

		Map map = gameObject.GetComponent<Map> ();

		if (Input.GetKeyDown (KeyCode.Y)) {
			map.SetTile (14, 0);
			map.RecalculateSurround ();
			MapBuilder mb = gameObject.GetComponent<MapBuilder> ();
			mb.ProcessMap ();
		}
	}
}
