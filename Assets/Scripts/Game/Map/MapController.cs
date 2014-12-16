using UnityEngine;
using System.Collections;
using System;
using RR2.Map;
using System.Collections.Generic;


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
		// TODO this is only here for testing
		// Remove in the final version
		if (Input.GetKeyDown (KeyCode.Keypad1)) {
			MapController.Instance.DrillTile(13);
		}
			//MapController.Instance.SetTile (13, 0);
	}

	public void DrillTile(int tileId) {
		GameTileType temp = GetTile (tileId);
		if (temp.TileTypeDestroysIntoSpecified) {
			SetTile(tileId, temp.TileTypeDestroysInto);
		}
		if (temp.PathTypeDestroysIntoSpecified) {
			SetPath(tileId, temp.PathTypeDestroysInto);
		}
	}

	public void SelectTile(int tileId) {
		Debug.Log(string.Format("Selecting tile {0}", tileId));
		// Deselect currently selected tiles
		if (TileSelected >= 0)
			DeselectTile ();

		// Change the colour of the tile
		SetTileColour (tileId, Color.green);

		TileSelected = tileId;

	}

	public void DeselectTile() {
		if (TileSelected >= 0) {
			Debug.Log (string.Format ("Deselecting tile {0}", TileSelected));

			SetTileColour (TileSelected, Color.white);

			TileSelected = -1;
		}
	}

	public void SetPath(int tileId, int pathId) {
		// TODO SetPath
		Debug.Log (string.Format ("Setting path at {0} to {1}", tileId, pathId));

		GetComponent<GameMap> ().Tiles [tileId].PathTypeIdSpecified = true;
		GetComponent<GameMap> ().Tiles [tileId].PathTypeId = pathId;

		MeshRenderer mr = GetComponent<MeshRenderer> ();
		mr.materials[tileId].SetTexture ("_PathTex", GetComponent<GameMap>().GamePathTypes[pathId].Texture);

		if (GetComponent<GameMap> ().GamePathTypes [pathId].Shoveled) {
			LogicController.Instance.AddJob(shovel: tileId);
		}
	}

	public GameTileType GetTile(int tileId) {
		//Debug.Log (string.Format("Getting tile {0}", tileId));
		Debug.Log (string.Format("Getting tile at {0}", tileId));

		// Try to get the tile type
		// Otherwise throw an exception
		if (tileId < Width * Height) {
			return GetComponent<GameMap> ().Tiles [tileId];
		} else {
			Debug.LogError("Tile out of range!");
			throw new IndexOutOfRangeException ();
		}
	}

	public void SetTile(int tileId, int tileTypeId) {
		Debug.Log (string.Format ("Setting tile at {0} to type {1}", tileId, tileTypeId));

		// Set the type of the particular tile on the map
		GetComponent<GameMap> ().SetTile (tileId, tileTypeId);
	}

	public float[] GetTileHeight(int tileId){
		// Return the height of the tile
		return new float[] { GetComponent<GameMap> ().Tiles [4 * tileId].Height,
			GetComponent<GameMap> ().Tiles [4 * tileId + 1].Height,
			GetComponent<GameMap> ().Tiles [4 * tileId + 2].Height,
			GetComponent<GameMap> ().Tiles [4 * tileId + 3].Height };
	}

	private void SetTileColour(int tileId, Color col) {
		// Set the colour of the tile
		MeshRenderer mr = GetComponent<MeshRenderer> ();
		mr.materials [tileId].color = col;
	}
}
