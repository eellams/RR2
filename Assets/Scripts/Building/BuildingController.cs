using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class BuildingController : MonoBehaviour {
	public Dictionary<int, BuildingType> BuildingTypes;
	public Dictionary<int, GameBuilding> GameBuildings;

	private int NextBuildingId = 0;

	private static BuildingController instance = null;
	public static BuildingController Instance {
		get
		{
			if (instance == null)
			{
				instance = (BuildingController)FindObjectOfType(typeof(BuildingController));
			}
			return instance;
		}
	}

	public void Initialise(XmlMap xmlMap) {
		BuildingTypes = new Dictionary<int, BuildingType> ();
		GameBuildings = new Dictionary<int, GameBuilding> ();
		
		foreach (BuildingType bType in xmlMap.BuildingTypes) {
			BuildingTypes.Add (bType.BuildingTypeId, bType);
		}


		foreach (Building building in xmlMap.Buildings) {
			AddBuilding(building.BuildingId, building.BuildingTypeId, building.TileX, building.TileZ);
		}
	}
	
	public void AddBuilding(int buildingId, int buildingTypeId, int tileX, int tileZ) {
		GameObject obj = (GameObject)Instantiate (Resources.Load (BuildingTypes [buildingTypeId].Model));

		GameBuilding gb = obj.GetComponent<GameBuilding> ();

		int mapWidth = MapController.Instance.GetComponent<Map> ().Width;
		int tileNumber = mapWidth * tileZ + tileX;
		float baseHeight = MapController.Instance.GetComponent<Map> ().Tiles [tileNumber].Height;

		MapController.Instance.GetComponent<Map> ().SetPath (tileNumber, BuildingTypes [buildingTypeId].BasePath);

		Vector3 position = Vector3.zero;
		// TODO not rely on 4.0f
		position.x = tileX * 4.0f + 2.0f;
		position.y = baseHeight * MapController.Instance.GetComponent<MapBuilder> ().HeightMultiplier;
		position.z = tileZ * 4.0f + 2.0f;

		obj.transform.parent = this.transform;
		// TODO position buildings
		obj.transform.position = position;

		if (buildingId > -1) {
			gb.BuildingId = buildingId;
			if (buildingId > NextBuildingId) {
				NextBuildingId = buildingId;
			}
		} else {
			gb.BuildingId = NextBuildingId++;
		}
	}
}
