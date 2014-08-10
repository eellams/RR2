using UnityEngine;
using System.Collections;
using System.Collections.Generic;

//[RequireComponent (typeof (CharacterControls))]

public class GameUnit : MonoBehaviour {
	public UnitType unitType;
	public GameObject Model;
	public int UnitId;
	public float PathMoveMulti = 1.0f;

	private List<UnitTask> Tasks = new List<UnitTask> ();
	private bool Selected;
	private bool Moving = false;
	private float Gravity = 9.8f;
	private float vSpeed = 0;

	void Start () {
		Deselect ();
		tag = "Unit";
	}
	
	// Update is called once per frame
	void Update () {
		Vector3 dirToMove = Vector3.zero;

		Map map = MapController.Instance.GetMap ();
		int tileNumber = Mathf.FloorToInt (transform.position.z / 4) * map.Width
						+ Mathf.FloorToInt (transform.position.x / 4);

		UnitMover um = GetComponent<UnitMover> ();

		if (map.Initialised) {
			if (map.Tiles [tileNumber].PathTypeId > 0) {
				PathMoveMulti = map.PathTypeDict [map.Tiles [tileNumber].PathTypeId].MoveMulti;
			} else {
				PathMoveMulti = 1.0f;

			}
			um.PathMulti = PathMoveMulti;
		}

		// If the unit has somewhere to move, move to it
		if (Tasks.Count > 0) {
			if (Tasks[0].position != Vector3.zero) {
				Vector3 position = transform.position;

				if ((!um.moveToTarget) && (!Moving)) {
					um.targetPosition = Tasks[0].position;
					um.moveToTarget = true;
					Moving = true;
				} else if ((Moving) && (!um.moveToTarget)) {
					Tasks.RemoveAt(0);
					Moving = false;
				}

			}

			else if (Tasks[0].pickup >= 0) {
				// TODO pickup object
			}

			else if (Tasks[0].drill >= 0) {
				// TODO drill object

				map.Tiles[Tasks[0].drill].DrillTime -= Time.deltaTime;

				if (map.Tiles[Tasks[0].drill].DrillTime < 0.0f) {
					map.SetTile (Tasks[0].drill, 0);
					map.RecalculateSurround ();
					MapBuilder mb = GameObject.Find ("Map").GetComponent<MapBuilder> ();
					mb.ProcessMap ();

					Tasks.RemoveAt (0);
				}
			}
		}
	}

	public void AddTask(UnitTask unitTask) {
		Tasks.Add (unitTask);
	}

	// Add a destination that this unit should move to
	public void AddMove (Vector3 position) {
		UnitTask toadd = new UnitTask ();
		toadd.position = position;
		AddTask (toadd);
	}

	// Clear the list of destinations
	public void ClearMove () {
		Tasks.Clear ();
	}

	// Does what it says on the tin
	public void ToggleSelect() {
		if (Selected)
			Deselect ();
		else
			Select ();
	}
	
	// Enables the select green thing
	public void Select() {
		Selected = true;
		transform.Find ("Select").gameObject.SetActive (true);
	}

	// And disables it again
	public void Deselect() {
		Selected = false;
		transform.Find ("Select").gameObject.SetActive (false);
	}
}

public class UnitTask {
	public Vector3 position;
	public int pickup;
	public int drill;

	// TODO closeness?
	
	public UnitTask() {
		this.pickup = -1;
		this.drill = -1;
	}
};