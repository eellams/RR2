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

	// Use this for initialization
	void Start () {
		Deselect ();
		tag = "Unit";
	}
	
	// Update is called once per frame
	void Update () {
		if (Selected) {
			GameObject cam = GameObject.Find ("Main Camera");

			GameObject select = transform.FindChild ("Select").gameObject;
			select.transform.LookAt (cam.transform.position);
		}

		Vector3 dirToMove = Vector3.zero;

		if (PointsToMove.Count > 0) {
			dirToMove = PointsToMove [0] - transform.position;
			dirToMove.y = 0.0f;

			Vector3 position = transform.position;
			if ((Mathf.Abs (position.x - PointsToMove [0].x) < 0.2f) &&
			(Mathf.Abs (position.y = PointsToMove [0].y) < 0.2f)) {
				PointsToMove.RemoveAt (0);
			}
		}

		CharacterControls cc = GetComponent<CharacterControls> ();

		if ((dirToMove.magnitude <= 0.1f) && (cc.grounded)) {
			cc.Stop ();
		} else {
			dirToMove.Normalize ();
			cc.targetVelocity = dirToMove;
		}
	}

	public void AddMove (Vector3 position) {
		PointsToMove.Add (position);
	}

	public void ClearMove () {
		PointsToMove.Clear ();
	}

	public void ToggleSelect() {
		if (Selected)
			Deselect ();
		else
			Select ();
	}

	public void Select() {
		Selected = true;
		for (int i=0; i<this.transform.childCount; i++) {
			if ((this.transform.GetChild (i).name == "Select")) {
				this.transform.GetChild (i).gameObject.SetActive (true);
			}
		}
	}

	public void Deselect() {
		Selected = false;
		for (int i=0; i<this.transform.childCount; i++) {
			if ((this.transform.GetChild (i).name == "Select")) {
				this.transform.GetChild (i).gameObject.SetActive (false);
			}
		}
	}
}
