using UnityEngine;
using System.Collections;

public class CameraCollider : MonoBehaviour {
	// Update is called once per frame

	void Update () {
		Ray ray = Camera.main.ScreenPointToRay (Input.mousePosition);
		RaycastHit hit;

		// TODO lots!

		// Do the raycasting
		if (Physics.Raycast (ray, out hit)) {
			// If left mouse button
			if (Input.GetMouseButtonDown (0)) {

				// If hits a unit
				if (hit.collider.gameObject.CompareTag ("Unit")) {
					// Find the unit which was clicked on
					GameUnit unitHit = hit.collider.gameObject.GetComponent<GameUnit>();

					// Select (or deselect) the unit
					UnitController.Instance.MouseSelect(unitHit.UnitId);
				}

				// If hits part of the map
				else if (hit.collider.CompareTag ("Map")) {
					// Get the map
					Map map = MapController.Instance.GetMap ();

					// Calculate the tile number
					int tileNumber = Mathf.FloorToInt (hit.point.z / 4.0f) * map.Width
						+ Mathf.FloorToInt (hit.point.x / 4.0f);

					UnitController.Instance.MapClick(tileNumber, hit.point);
				}
			}

			if (Input.GetMouseButtonDown (2)) {
				GameObject x = (GameObject)Instantiate (Resources.Load ("boomy"));
				boom y = x.GetComponent<boom>();
				x.transform.position = hit.point;
				y.pow ();
			}
		}


	}
}
