using UnityEngine;
using System.Collections;
using RR2.Statics;
using System.Collections.Generic;
using System.Reflection;
using RR2.Raider_;

public class RaiderController : Singleton<RaiderController> {
	protected RaiderController () {}

	public Dictionary<int, GameObject> GameRaiders;

	public List<int> RaidersSelected;

	private int NextRaiderId = 1;

	// Load stuff from the map etc.
	void Awake () {
		RockRaider rockRaider = ApplicationModel.Instance.MapData.RockRaider;
		Raider[] raiders = ApplicationModel.Instance.MapData.MapData.Raiders;

		GameRaiders = new Dictionary<int, GameObject> ();
		RaidersSelected = new List<int> ();

		Debug.Log (string.Format ("Adding {0} raiders from file", raiders.Length));

		foreach (Raider raider in raiders) {
			UnityEngine.Vector3 addpos = Vector3.zero;
			addpos.x = raider.Position.x;
			addpos.y = raider.Position.y;
			addpos.z = raider.Position.z;

			AddNewRaider(addpos, raider.RaiderId);
		}
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	// Select a raider by ID
	public void SelectRaider(int raiderId, bool addSelect=false) {
		if (!addSelect)
			DeselectAll ();
		GameRaiders [raiderId].GetComponent<GameRaider> ().Select ();
		RaidersSelected.Add (raiderId);
	}

	// Deselect a raider by ID
	public void DeselectRaider(int raiderId, bool addSelect=false) {
		if (!addSelect) {
			DeselectAll();
		} else {
			GameRaiders [raiderId].GetComponent<GameRaider> ().Deselect ();
			RaidersSelected.Remove (raiderId);
		}
	}

	// Deselect all raiders
	public void DeselectAll() {
		Debug.Log ("Deselecting all raiders");
		while (RaidersSelected.Count > 0) {
			DeselectRaider(RaidersSelected[0], true);
		}
	}

	// Add a new raider at a specified location
	public void AddNewRaider(Vector3 position, int raiderId = 0) {
		Debug.Log (string.Format ("Adding new raider {0} at {1}", raiderId, position));

		GameObject toAdd = CreateRaider (ApplicationModel.Instance.MapData.RockRaider.ModelPath);
		
		toAdd.transform.position = position;

		// Adjust the id of the next raider to be added (if appropriate)
		if (raiderId > NextRaiderId) NextRaiderId = raiderId;

    	toAdd.GetComponent<GameRaider> ().GR.RaiderId = NextRaiderId;
		NextRaiderId++;

		GameRaiders.Add (toAdd.GetComponent<GameRaider> ().RaiderId, toAdd);

	}

	private GameObject CreateRaider(string ResourcePath) {
		Debug.Log ("Creating Raider");
		GameObject toReturn = (GameObject)Instantiate(Resources.Load(ResourcePath));
		toReturn.AddComponent<GameRaider>();
		toReturn.transform.position = UnityEngine.Vector3.zero;
		toReturn.transform.parent = this.transform;
		toReturn.tag = "Raider";
		return toReturn;
	}
}
