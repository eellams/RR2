using UnityEngine;
using System.Collections;
using RR2.Map;
using RR2.Raider_;

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
				Debug.Log("Mouse button 0 pressed");

				// Calculates the tile number from the clicked position
				int tileNumber = Mathf.FloorToInt (hit.point.z / MapController.Instance.TileSize) *
					MapController.Instance.Width +
					Mathf.FloorToInt (hit.point.x / MapController.Instance.TileSize);

				// Clicked on a map tile
				if (hit.collider.CompareTag("Map")) {
					Debug.Log(string.Format("Clicked on tile {0}", tileNumber));

					RaiderController.Instance.DeselectAll();

					// TODO some tiles should be 'unselectable'

					if (MapController.Instance.TileSelected != tileNumber) {
						MapController.Instance.SelectTile(tileNumber);
					} else {
						MapController.Instance.DeselectTile();
					}
				} else if (hit.collider.CompareTag("Raider")) {
					int raiderId = hit.collider.gameObject.GetComponent<GameRaider>().RaiderId;

					Debug.Log(string.Format("Clicked on raider {0}", raiderId));

					MapController.Instance.DeselectTile();

					bool addSelect = Input.GetKey(KeyCode.LeftControl) || Input.GetKey(KeyCode.RightControl);

					if (!RaiderController.Instance.RaidersSelected.Contains(raiderId)) {
						RaiderController.Instance.SelectRaider(raiderId, addSelect);
					} else {
						RaiderController.Instance.DeselectRaider(raiderId, addSelect);
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
