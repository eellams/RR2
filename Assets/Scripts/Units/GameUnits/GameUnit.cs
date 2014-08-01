using UnityEngine;
using System.Collections;
using System.Collections.Generic;

[RequireComponent (typeof (CharacterControls))]

public class GameUnit : MonoBehaviour {
	public UnitType unitType;
	public GameObject Model;
	public int UnitId;

	private List<UnitTask> Tasks = new List<UnitTask> ();
	private bool Selected;
	private float Gravity = 9.8f;
	private float vSpeed = 0;

	void Awake () {
		Deselect ();
		tag = "Unit";
	}
	
	// Update is called once per frame
	void Update () {
		// If selected, billboard the select node so that it always faces
		// the camera
		/*if (Selected) {
			GameObject cam = GameObject.Find ("Main Camera");

			GameObject select = transform.FindChild ("Select").gameObject;
			select.transform.LookAt (cam.transform.position);
		}*/

		Vector3 dirToMove = Vector3.zero;

		// If the unit has somewhere to move, move to it
		if (Tasks.Count > 0) {
			if (Tasks[0].position != Vector3.zero) {
				dirToMove = Tasks [0].position - transform.position;
				dirToMove.y = 0.0f;

				Vector3 position = transform.position;

				// If the unit has arrived at a particular destination,
				// remove it from the list of destinations
				if ((Mathf.Abs (position.x - Tasks [0].position.x) < 0.2f) &&
				(Mathf.Abs (position.y = Tasks [0].position.y) < 0.2f)) {
					Tasks.RemoveAt (0);
				}
			}

			else if (Tasks[0].pickup >= 0) {
				// TODO pickup object
			}

			else if (Tasks[0].drill >= 0) {
				// TODO drill object
				Map map = GameObject.Find ("Map").GetComponent<Map>() ;

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

		// The character controls controls the movement of the character
		// (duh ??)
		CharacterControls cc = GetComponent<CharacterControls> ();

		// If the unit has stopped moving, ensure it is stopped
		// otherwuse move the unit.
		if ((dirToMove.magnitude < 0.2f) && (cc.grounded)) {
			cc.Stop ();
		} else {
			dirToMove.Normalize ();
			cc.targetVelocity = dirToMove;
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