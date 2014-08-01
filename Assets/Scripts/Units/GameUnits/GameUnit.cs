using UnityEngine;
using System.Collections;
using System.Collections.Generic;

[RequireComponent (typeof (CharacterControls))]

public class GameUnit : MonoBehaviour {
	public UnitType unitType;
	public GameObject Model;
	public int UnitId;

	private List<Vector3> PointsToMove = new List<Vector3> ();
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
		if (Selected) {
			GameObject cam = GameObject.Find ("Main Camera");

			GameObject select = transform.FindChild ("Select").gameObject;
			select.transform.LookAt (cam.transform.position);
		}

		Vector3 dirToMove = Vector3.zero;

		// If the unit has somewhere to move, move to it
		if (PointsToMove.Count > 0) {
			dirToMove = PointsToMove [0] - transform.position;
			dirToMove.y = 0.0f;

			Vector3 position = transform.position;

			// If the unit has arrived at a particular destination,
			// remove it from the list of destinations
			if ((Mathf.Abs (position.x - PointsToMove [0].x) < 0.2f) &&
			(Mathf.Abs (position.y = PointsToMove [0].y) < 0.2f)) {
				PointsToMove.RemoveAt (0);
			}
		}

		// The character controls controls the movement of the character
		// (duh ??)
		CharacterControls cc = GetComponent<CharacterControls> ();

		// If the unit has stopped moving, ensure it is stopped
		// otherwuse move the unit.
		if ((dirToMove.magnitude <= 0.1f) && (cc.grounded)) {
			cc.Stop ();
		} else {
			dirToMove.Normalize ();
			cc.targetVelocity = dirToMove;
		}
	}

	// Add a destination that this unit should move to
	public void AddMove (Vector3 position) {
		PointsToMove.Add (position);
	}

	// Clear the list of destinations
	public void ClearMove () {
		PointsToMove.Clear ();
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
