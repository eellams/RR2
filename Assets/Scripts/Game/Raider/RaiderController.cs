using UnityEngine;
using System.Collections;
using RR2.Statics;
using System.Collections.Generic;
using System.Reflection;
using RR2.Raider_;

public class RaiderController : Singleton<RaiderController> {
	protected RaiderController () {}

	public Dictionary<int, GameObject> GameRaiders;

	private int NextRaiderId = 1;


	void Awake () {
		RockRaider rockRaider = ApplicationModel.Instance.MapData.RockRaider;
		Raider[] raiders = ApplicationModel.Instance.MapData.MapData.Raiders;

		GameRaiders = new Dictionary<int, GameObject> ();

		foreach (Raider raider in raiders) {
			/*GameObject toadd = (GameObject)Instantiate(Resources.Load(rockRaider.ModelPath));

			toadd.AddComponent<GameRaider>();

			toadd.GetComponent<GameRaider>().GR.InheritFromRaider(raider);
			Debug.Log(toadd.GetComponent<GameRaider>().GR.Name);*/
			//GameObject toadd = CreateRaider(rockRaider.ModelPath);

			UnityEngine.Vector3 addpos = Vector3.zero;
			addpos.x = raider.Position.x;
			addpos.y = raider.Position.y;
			addpos.z = raider.Position.z;

			AddNewRaider(addpos, raider.RaiderId);

			//toadd.transform.position = addpos;
			//toadd.transform.parent = transform;

			//GameRaiders.Add(toadd.GetComponent<GameRaider>().RaiderId, toadd);

			/*GameRaider toAdd = new GameTileType(tileType);
			
			toAdd.Texture = LoadTexture(toAdd.TexturePath);
			
			GameTileTypes.Add(toAdd.TileTypeId, toAdd);*/
			//AddNewRaider
		}
		Vector3 temp = Vector3.zero;
		AddNewRaider (temp);
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	public void AddNewRaider(Vector3 position, int raiderId = 0) {
		GameObject toAdd = CreateRaider (ApplicationModel.Instance.MapData.RockRaider.ModelPath);
		
		toAdd.transform.position = position;

		if (raiderId > 0) {
			if (raiderId > NextRaiderId) {
				NextRaiderId = raiderId;//++;
			}
		}
    	toAdd.GetComponent<GameRaider> ().GR.RaiderId = raiderId;
		raiderId++;

		GameRaiders.Add (toAdd.GetComponent<GameRaider> ().RaiderId, toAdd);

	}

	private GameObject CreateRaider(string ResourcePath) {
		GameObject toReturn = (GameObject)Instantiate(Resources.Load(ResourcePath));
		toReturn.AddComponent<GameRaider>();
		toReturn.transform.position = UnityEngine.Vector3.zero;
		toReturn.transform.parent = this.transform;
		return toReturn;
	}
}
