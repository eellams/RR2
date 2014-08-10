using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class ObjectController : MonoBehaviour {
	public Dictionary<int, ObjectType> ObjectTypeDict;
	public Dictionary<int, GameObject_> GameObjects_;

	private int NextObjId = 0;

	private static ObjectController instance = null;
	public static ObjectController Instance {
		get
		{
			if (instance == null)
			{
				instance = (ObjectController)FindObjectOfType(typeof(ObjectController));
			}
			return instance;
		}
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

		GameObject_ go = obj.GetComponent<GameObject_> ();
		
		// Reparent it
		obj.transform.parent = this.transform;
		obj.transform.position = position;

		if (objectId > -1) {
			go.ObjectId = objectId;
			if (objectId > NextObjId) {
					NextObjId = objectId;
			}
		} else {
			go.ObjectId = NextObjId++;
		}
		
		// Add it to the list of game units
		GameObjects_ [objectId] = go;
	}
}
