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

	public Dictionary<TaskTypes, List<GameTask>> Tasks;

	public enum MoveType { Land=1, Air=3, Water=2 };
	public enum TaskTypes {Shovel};

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void Awake() {
		BuildingRequires = new Dictionary<int, GameRequires> ();
		VehicleRequires = new Dictionary<int, GameRequires> ();
		ObjectsCanAcceptDrop = new Dictionary<int, GameDropRequires> ();
		ToolsCanAcceptDrop = new Dictionary<int, GameDropRequires> ();

		Tasks = new Dictionary<TaskTypes, List<GameTask>> ();
		foreach (TaskTypes taskType in (TaskTypes[]) Enum.GetValues(typeof(TaskTypes)))
		{
			Tasks.Add(taskType, new List<GameTask>());
		}
	}

	public void SetAStar(int tileNumber, MoveType graphType, bool walkable) {
		throw new NotImplementedException ();
	}

	public void SetAStarPos(int tileNumber, Vector3 position) {
		throw new NotImplementedException ();
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

	public void AddTask(TaskTypes taskType, int dataId, int taskId = 0) {
		GameTask gameTask = new GameTask();

		//TODO lots of these to add
		switch (taskType) {
		case TaskTypes.Shovel:
			Debug.Log(string.Format("Adding task to shovel path at {0}", dataId));
			gameTask.DataId = dataId;
			break;
		}

		Tasks[taskType].Add(gameTask);
	}
}

public class GameTask {
	public LogicController.TaskTypes TaskType;
	public int DataId;
}

public class GameRequires : Requires {

}

public class GameDropRequires : DropRequires {

}
