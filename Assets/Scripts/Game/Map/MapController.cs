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
		if (Input.GetKeyDown (KeyCode.Keypad1))
			MapController.Instance.SetTile (13, 0);
	}

	public void SelectTile(int tileId) {
		Debug.Log(string.Format("Selecting tile {0}", tileId));
		if (TileSelected >= 0)
			DeselectTile ();
		MeshRenderer mr = GetComponent<MeshRenderer> ();
		mr.materials [tileId].color = Color.green;
		TileSelected = tileId;

	}

	public void DeselectTile() {
		if (TileSelected >= 0) {
			Debug.Log (string.Format ("Deselecting tile {0}", TileSelected));
			MeshRenderer mr = GetComponent<MeshRenderer> ();
			mr.materials [TileSelected].color = Color.white;
			TileSelected = -1;
		}
	}

	public void SetPath(int tileId, int pathId) {
		Debug.Log (string.Format ("Setting tile at {0} to {1}"));
		throw new NotImplementedException();
	}

	public GameTileType GetTile(int tileId) {
		Debug.Log (string.Format("Getting tile {0}", tileId));

		if (tileId < Width * Height) {
			return GetComponent<GameMap> ().Tiles [tileId];
		} else {
			Debug.LogError("Tile out of range!");
			throw new IndexOutOfRangeException ();
		}
	}

	public void SetTile(int tileId, int tileTypeId) {
		GetComponent<GameMap> ().SetTile (tileId, tileTypeId);
	}
}
