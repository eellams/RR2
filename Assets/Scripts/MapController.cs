using UnityEngine;
using System.Collections;
using System;
using System.IO;

public class MapController : MonoBehaviour {
	public Material baseMaterial;

	private Map map;

	// Use this for initialization
	void Start () {
		// Set up the required components
		MapReader mapReader = gameObject.AddComponent<MapReader> ();
		MapBuilder mapBuilder = gameObject.AddComponent<MapBuilder> ();

		MeshFilter meshFilter = gameObject.AddComponent<MeshFilter> ();
		MeshRenderer meshRenderer = gameObject.AddComponent<MeshRenderer> ();
		MeshCollider meshCollider = gameObject.AddComponent<MeshCollider> ();

		// Load the map
		map = mapReader.LoadMap (Path.Combine (Environment.CurrentDirectory, "Data/testmap.xml"));
		map.Setup (baseMaterial);

		// Build the map
		mapBuilder.ProcessMap (map);
	}
	
	// Update is called once per frame
	void Update () {
		// TODO avalanche, corrosion etc.

		if (Input.GetKeyDown (KeyCode.Y)) {
			map.SetTile (14, 0);
			map.RecalculateSurround();
			MapBuilder mb = gameObject.GetComponent<MapBuilder>();
			mb.ProcessMap (map);
		}
	}
}
