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

	private static MapController instance = null;
	public static MapController Instance {
		get
		{
			if (instance == null)
			{
				instance = (MapController)FindObjectOfType(typeof(MapController));
			}
			return instance;
		}
	}

	public Map GetMap() {
		return (Map)FindObjectOfType (typeof(Map));
	}

	// Use this for initialization
	void Start () {
		// Get the required components
		MapReader mapReader = GetComponent<MapReader> ();
		Map map = MapController.Instance.GetMap ();
		MapBuilder mapBuilder = GetComponent<MapBuilder> ();

		// Load the map
		XmlMap xmlMap = mapReader.LoadMap (Path.Combine (Environment.CurrentDirectory, "Data/testmap.xml"));

		// TODO this doesn't seem right
		GameObject unitGameobject = GameObject.Find ("Unit");

		NewAstar aStar = unitGameobject.GetComponent<NewAstar> ();

		// Use required data from xmlMap
		UnitController.Instance.Initialise (xmlMap);

		GameObject worldGameobject = GameObject.Find ("World");

		ObjectController.Instance.Initialise (xmlMap);



		// Initialise the map (use required data from xmlMap)
		map.Initialise (baseMaterial, xmlMap);

		BuildingController.Instance.Initialise (xmlMap);

		// Initialise the A* path finding
		aStar.Initialise ();

		// Build the map
		// Also recalculates A*, which is nice ^_^
		mapBuilder.ProcessMap ();
	}
	
	// Update is called once per frame
	void Update () {
		// TODO avalanche, corrosion etc.

		Map map = MapController.Instance.GetMap ();

		if (Input.GetKeyDown (KeyCode.Y)) {
			map.SetTile (14, 0);
			map.RecalculateSurround ();
			MapBuilder mb = gameObject.GetComponent<MapBuilder> ();
			mb.ProcessMap ();
		}
	}
}
