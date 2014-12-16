using UnityEngine;
using System.Collections;
using RR2.Statics;
using System.Collections.Generic;
using System;

public class LogicController : Singleton<LogicController> {
	protected LogicController () {}

	//public GameRequires RaiderRequires;
	public Dictionary<int, GameRequires> BuildingRequires;
	public Dictionary<int, GameRequires> VehicleRequires;
	//public Dictionary<int, GameRequires> PathRequires;
	//public Dictionary<int, GameRequires> UpgradeRequires;

	public Dictionary<int, GameDropRequires> ObjectsCanAcceptDrop;
	public Dictionary<int, GameDropRequires> ToolsCanAcceptDrop;

	public Dictionary<int, List<GameTask>> Tasks;

	public enum MoveType { Land=1, Air=3, Water=2 };

	private int NextTaskId = 0;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	public void SetAStar(int tileNumber, MoveType graphType, bool walkable) {

	}

	public void SetAStarPos(int tileNumber, Vector3 position) {

	}



	public List<Vector3> GetPath(Vector3 start, Vector3 end) {
		throw new NotImplementedException ();
	}

	public bool CanAddVehicle(int vehicleTypeId) {
		throw new NotImplementedException();
	}

	public bool CanAddRaider() {
		throw new NotImplementedException();
	}

	public bool CanDropObject(int objectTypeId) {
		throw new NotImplementedException();
	}

	public bool CanAcceptObject(int objectTypeId) {
		throw new NotImplementedException();
	}

	public bool CanDropTool(int toolTypeId) {
		throw new NotImplementedException();
	}

	public bool CanGiveUpgrade(int unitUpgradeId) {
		throw new NotImplementedException ();
	}

	public List<int> AvailableBuildings() {
		throw new NotImplementedException ();
	}

	public void AddJob(int shovel = -1) {
		throw new NotImplementedException ();
	}
}

public class GameTask {

}

public class GameRequires : Requires {

}

public class GameDropRequires : DropRequires {

}
