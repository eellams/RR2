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

		map = mapReader.LoadMap (Path.Combine (Environment.CurrentDirectory, "Data/testmap.xml"));
		map.Setup (baseMaterial);

		mapBuilder.ProcessMap (map);
	}
	
	// Update is called once per frame
	void Update () {
		// TODO avalanche, corrosion etc.
	}
}
