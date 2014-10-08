using UnityEngine;
using System.Collections;
using System;
using RR2.Map;


[RequireComponent (typeof(GameMap))]
[RequireComponent (typeof(MeshFilter))]
public class MapController : Singleton<MapController> {
	protected MapController () {}

	public int Width { get { return GetComponent<GameMap> ().Width; } }
	public int Height { get { return GetComponent<GameMap> ().Height; } }
	public float TileSize { get { return GetComponent<GameMap> ().TileSize; } }

	public int TileSelected = -1;

	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {

	}

	public void SelectTile(int tileId) {
		if (TileSelected >= 0)
			DeselectTile (TileSelected);
		MeshRenderer mr = GetComponent<MeshRenderer> ();
		mr.materials [tileId].color = Color.green;
		TileSelected = tileId;

	}

	public void DeselectTile(int tileId) {
		MeshRenderer mr = GetComponent<MeshRenderer> ();
		mr.materials [tileId].color = Color.white;
		TileSelected = -1;
	}

	public void SetPath(int tileId, int pathId) {
		throw new NotImplementedException();
	}

	public void GetTile(int tileId) {
		throw new NotImplementedException();
	}
}
