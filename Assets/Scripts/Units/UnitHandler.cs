using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using EMK.Cartography;

[RequireComponent (typeof (NewAstar))]
public class UnitHandler : MonoBehaviour {
	public Dictionary<int, UnitType> UnitTypes;

	public Dictionary<int, GameUnit> GameUnits;

	private int UnitId;
	private List<int> Selected;

	// Use this for initialization
	void Start () {
		Selected = new List<int>();
	}
	
	// Update is called once per frame
	void Update () {
	}

	public void Initialise (XmlMap xmlMap) {
		UnitTypes = new Dictionary<int, UnitType> ();
		GameUnits = new Dictionary<int, GameUnit> ();

		// Add the different unit types to the dictionary
		for (int i=0; i<xmlMap.UnitTypes.Length; i++) {
			UnitTypes.Add (xmlMap.UnitTypes[i].UnitTypeId, xmlMap.UnitTypes[i]);
		}

		for (int i=0; i<xmlMap.Units.Length; i++) {
			// Set the position of the unit
			Vector3 position = Vector3.zero;
			position.x = xmlMap.Units[i].PosX;
			position.y = xmlMap.Units[i].PosY;
			position.z = xmlMap.Units[i].PosZ;

			AddUnit (xmlMap.Units[i].UnitId, xmlMap.Units[i].UnitTypeId, position);
		}
	}

	public void AddUnit (int UnitId, int unitTypeId, Vector3 position) {
		GameObject unit = new GameObject ();

		switch (UnitTypes [unitTypeId].Model) {
			case "Minifig":
				unit = (GameObject)Instantiate (Resources.Load ("Models/Minifig/Minifig"));
				break;
		}

		GameUnit gameUnit = unit.AddComponent<GameUnit> ();
		unit.transform.parent = this.transform;
		unit.transform.position = position;
		
		gameUnit.UnitId = UnitId;
		
		GameUnits[UnitId] = gameUnit;
	}

	public void MoveAStar (int GameUnitId, Vector3 endPos, bool addToList) {
		NewAstar nas = GetComponent<NewAstar> ();
		Vector3 startPos = GameUnits [GameUnitId].transform.position;

		Arc[] list = nas.Calculate (startPos, endPos);

		if (!addToList)
			GameUnits [GameUnitId].ClearMove ();

		foreach (Arc arc in list) {
			GameUnits[GameUnitId].AddMove (ArcEndToVector(arc));
		}
		if (list.Length > 0)
			GameUnits[GameUnitId].AddMove (endPos);
	}

	private Vector3 ArcEndToVector (Arc a) {
		return new Vector3 ((float)a.EndNode.X, 0.0f, (float)a.EndNode.Y);
	}

	public void MapClick (int tileNumber, Vector3 position) {
		Map map = GameObject.Find ("Map").GetComponent<Map> ();
		if ((Selected.Count > 0) && !map.Tiles[tileNumber].Solid) {
			foreach (int i in Selected) {
				MoveAStar (i, position, false);
			}
		}
	}

	public void MoveUnit (int GameUnitId, Vector3 position, bool addToList) {
		if (!addToList)
			GameUnits [GameUnitId].ClearMove ();
		GameUnits [GameUnitId].AddMove (position);
	}

	public void SelectUnit (int GameUnitId) {
		Selected.Add (GameUnitId);
		GameUnits [GameUnitId].Select ();
	}

	public void DeselectUnit (int GameUnitId) {
		Selected.Remove (GameUnitId);
		GameUnits [GameUnitId].Deselect ();
	}

	public void ToggleSelect (int GameUnitId) {
		if (Selected.Contains (GameUnitId))
			DeselectUnit (GameUnitId);
		else
			SelectUnit (GameUnitId);
	}

	public void DeselectAll () {
		for (int i=0; i<Selected.Count; i++) {
			DeselectUnit (Selected[i]);
		}
		Selected.Clear ();
	}

	public void MouseSelect (int GameUnitId) {
		if (Input.GetKey (KeyCode.LeftShift) || Input.GetKey (KeyCode.RightShift) || Selected.Contains (GameUnitId)) {
			ToggleSelect (GameUnitId);
		} else {
			DeselectAll ();
			SelectUnit (GameUnitId);
		}
	}
}
