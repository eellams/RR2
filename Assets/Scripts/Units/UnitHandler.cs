using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class UnitHandler : MonoBehaviour {
	public Dictionary<int, UnitType> UnitTypes;

	// Use this for initialization
	void Start () {

	}
	
	// Update is called once per frame
	void Update () {
		
	}

	public void Initialise (XmlMap xmlMap) {
		UnitTypes = new Dictionary<int, UnitType> ();

		// Add the different unit types to the dictionary
		for (int i=0; i<xmlMap.UnitTypes.Length; i++) {
			UnitTypes.Add (xmlMap.UnitTypes[i].UnitTypeId, xmlMap.UnitTypes[i]);
		}

		for (int i=0; i<xmlMap.Units.Length; i++) {
			//GameObject toadd = new GameObject();
			GameUnit gameUnit = gameObject.AddComponent<GameUnit>();
			gameUnit.unitType = UnitTypes[xmlMap.Units[i].UnitTypeId];
			gameUnit.transform.position = Vector3.zero;

			switch (gameUnit.unitType.Model) {
			case "Minifig":
				// Load the minifig model
				gameUnit.model = (GameObject)Instantiate (Resources.Load ("Models/Minifig/Minifig"));

				// Parent the minifig model correctly
				gameUnit.model.transform.parent = gameUnit.transform;
				break;
			}

			// Update the name of the gameobject accordingly
			gameUnit.model.name = xmlMap.Units[i].UnitId.ToString() + "_" +
				gameUnit.unitType.Name;

			// Set the position of the unit
			Vector3 position = Vector3.zero;
			position.x = xmlMap.Units[i].PosX;
			position.y = xmlMap.Units[i].PosY;
			position.z = xmlMap.Units[i].PosZ;
			
			gameUnit.model.transform.position = position;
		}
	}
}
