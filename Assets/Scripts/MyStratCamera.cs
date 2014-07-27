using UnityEngine;
using System.Collections;

public class MyStratCamera : MonoBehaviour {

	public Vector3 PanLimits = new Vector3 (100, 0, 100);
	public Vector3 MousePoint;
	public float CamDist = 10.0f;
	public float ScrollSpeed = 0.5f;
	public float ZoomSpeed = 50.0f;

	private GameObject RotatePoint;
	
	// Use this for initialization
	void Start () {
		// Create the new GameObject
		RotatePoint = new GameObject ("Camera Rotation Point");

		// Reparent this (the camera) to the rotation point
		this.transform.parent = RotatePoint.transform;

		// Initialise the mousepoint
		MousePoint = new Vector3 ();
	}

	private float Clamp(float value, float min, float max) {
		if (min > value) {
			return min;
		} else if (max < value) {
			return max;
		} else {
			return value;
		}
	}

	// A lot of maths goes on here which I haven't been bothered to learn properly.
	// It seems to work well-ish though
	private void TurnCameraAroundPoint(float deltaX, float deltaY) {
		Vector3 newCamHpr = new Vector3 ();
		Vector3 newCamPos = new Vector3 ();

		// The current HPR of the camera
		Vector3 camHpr = transform.eulerAngles;

		// Do some maths to calculate the new HPR
		newCamHpr.y = camHpr.y + deltaX;
		newCamHpr.x = Clamp(camHpr.x - deltaY, 10, 85);
		newCamHpr.z = camHpr.z;

		// Adjust the heading of the parent node
		Vector3 yHpr = Vector3.zero;
		yHpr.y = newCamHpr.y;
		RotatePoint.transform.eulerAngles = yHpr;

		// Rotate the camera
		transform.eulerAngles = newCamHpr;

		// Confusingly named variables to calculate stuff
		float angleradiansX = newCamHpr.y * (Mathf.PI / 180);
		float angleradiansY = newCamHpr.x * (Mathf.PI / 180);

		// Calculate the camera position
		// Basically works out where on a circle the camera should be, and then adds this point to the central vector (?)
		newCamPos.x = - CamDist * Mathf.Sin (angleradiansX) * Mathf.Cos (angleradiansY) + RotatePoint.transform.position.x;
		newCamPos.z = - CamDist * Mathf.Cos (angleradiansX) * Mathf.Cos (angleradiansY) + RotatePoint.transform.position.z;
		newCamPos.y = CamDist * Mathf.Sin (angleradiansY) + RotatePoint.transform.position.y;

		// Update the position of the camera
		transform.position = newCamPos;
	}

	// Sets the focal position of the camera. The position must be in global coordinates
	private void SetTarget (Vector3 position) {
		position.x = Clamp (position.x, 0, PanLimits.x);
		position.y = Clamp (position.y, 0, PanLimits.y);
		position.z = Clamp (position.z, 0, PanLimits.z);
		RotatePoint.transform.position = position;
	}

	// Move the camera along relative coordinates
	private void MoveTarget (Vector3 position) {
		RotatePoint.transform.Translate (position);
		SetTarget (RotatePoint.transform.position);
	}
	
	// Update is called once per frame
	void LateUpdate () {
		// The translation to be applied
		Vector3 translation = Vector3.zero;

		// Zoom in or out
		var zoomDelta = Input.GetAxis ("Mouse ScrollWheel") * ZoomSpeed * Time.deltaTime;
		if (zoomDelta!=0)
			CamDist -= zoomDelta;


		// Get the change in mouse point
		Vector3 delta = Input.mousePosition - MousePoint;
		// Update the stored value
		MousePoint = Input.mousePosition;

		// If right click, orbit the camera around the central point
		if (Input.GetMouseButton (1)) {
			TurnCameraAroundPoint (delta.x, delta.y);
		} else {
			// Else move the camera with arrow keys (or WASD etc. depending on the project input settings)
			translation += new Vector3(Input.GetAxis("Horizontal"), 0, Input.GetAxis("Vertical")) * ScrollSpeed;
			MoveTarget (translation);

			// Adjust for the height of objects and terrain below the camera
			RaycastHit hit;
			Vector3 rayOrigin = RotatePoint.transform.position;
			rayOrigin.y += 100.0f;

			Ray ray = new Ray (rayOrigin, Vector3.down);

			if (Physics.Raycast (ray, out hit)) {
				RotatePoint.transform.position = hit.point;
				// Update the camera
				TurnCameraAroundPoint (0,0);
			}
		}
	}
}
