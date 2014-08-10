using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class ObjectController : MonoBehaviour {
	public Dictionary<int, ObjectType> ObjectTypeDict;
	public Dictionary<int, GameObject_> GameObjects_;

	private int NextObjId = 0;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	public void Initialise(XmlMap xmlMap) {
		ObjectTypeDict = new Dictionary<int, ObjectType> ();
		GameObjects_ = new Dictionary<int, GameObject_> ();
		
		foreach (ObjectType oType in xmlMap.ObjectTypes) {
			ObjectTypeDict.Add (oType.ObjectTypeId, oType);
		}

		foreach (Object_ oType in xmlMap.Objects) {
			Vector3 position = new Vector3(oType.PosX, oType.PosY, oType.PosZ);
			AddObject (oType.ObjectId, oType.ObjectTypeId, position);
		}
	}

	public void AddObject(int objectId, int objectTypeId, Vector3 position) {
		GameObject obj = (GameObject)Instantiate (Resources.Load (ObjectTypeDict [objectTypeId].Model));
		
		//// Add the gameunit
		//GameUnit gameUnit = unit.AddComponent<> ();
		GameObject_ gameUnit = obj.GetComponent<GameObject_> ();
		
		// Reparent it
		obj.transform.parent = this.transform;
		obj.transform.position = position;

		if (objectId > -1) {
			gameUnit.ObjectId = objectId;
			if (objectId > NextObjId) {
					NextObjId = objectId;
			}
		} else {
			gameUnit.ObjectId = NextObjId++;
		}


		
		// Add it to the list of game units
		GameObjects_ [objectId] = gameUnit;
	}
}
