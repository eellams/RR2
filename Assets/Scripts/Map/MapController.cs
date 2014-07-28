using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Collections.Generic;

public class MapController : MonoBehaviour {
	public Material baseMaterial;

	// Use this for initialization
	void Start () {
		// Set up the required components
		MapReader mapReader = gameObject.AddComponent<MapReader> ();
		MapBuilder mapBuilder = gameObject.AddComponent<MapBuilder> ();

		MeshFilter meshFilter = gameObject.AddComponent<MeshFilter> ();
		MeshRenderer meshRenderer = gameObject.AddComponent<MeshRenderer> ();
		MeshCollider meshCollider = gameObject.AddComponent<MeshCollider> ();

		Map map = gameObject.AddComponent<Map> ();

		// Load the map
		XmlMap xmlMap = mapReader.LoadMap (Path.Combine (Environment.CurrentDirectory, "Data/testmap.xml"));

		// This is on a sepeate gameobject, it just seemed more sensible at the time
		GameObject unitGameobject = GameObject.Find ("Units");
		UnitHandler unitHandler = unitGameobject.GetComponent<UnitHandler> ();

		// Use required data from xmlMap
		unitHandler.Initialise (xmlMap);

		// Initialise the map (use required data from xmlMap)
		map.Initialise (baseMaterial, xmlMap);

		// Build the map
		mapBuilder.ProcessMap ();
	}
	
	// Update is called once per frame
	void Update () {
		// TODO avalanche, corrosion etc.

		Map map = gameObject.GetComponent<Map> ();

		if (Input.GetKeyDown (KeyCode.Y)) {
			map.SetTile (14, 0);
			map.RecalculateSurround();
			MapBuilder mb = gameObject.GetComponent<MapBuilder>();
			mb.ProcessMap ();
		}
	}
}
