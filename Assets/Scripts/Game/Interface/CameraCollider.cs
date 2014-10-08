using UnityEngine;
using System.Collections;
using RR2.Map;

public class CameraCollider : MonoBehaviour {
	// Update is called once per frame

	void Update () {
		Ray ray = Camera.main.ScreenPointToRay (Input.mousePosition);
		RaycastHit hit;

		// TODO lots!

		// Do the raycasting
		if (Physics.Raycast (ray, out hit)) {
			// If left mouse button
			if (Input.GetMouseButtonDown (0) && !Input.GetMouseButton(1)) {

				if (hit.collider.CompareTag("Map")) {
					int tileNumber = Mathf.FloorToInt (hit.point.z / MapController.Instance.TileSize) * MapController.Instance.Width
						+ Mathf.FloorToInt (hit.point.x / MapController.Instance.TileSize);
					if (MapController.Instance.TileSelected != tileNumber) {
						MapController.Instance.SelectTile(tileNumber);
					} else {
						MapController.Instance.DeselectTile(tileNumber);
					}
				}

				/*// If hits a unit
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
				}*/
			}
		}


	}
}
