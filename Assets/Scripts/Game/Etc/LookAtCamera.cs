using UnityEngine;
using System.Collections;

public class LookAtCamera : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		GameObject cam = GameObject.Find ("Main Camera");
		
		transform.LookAt( cam.transform.position ); 
		
		transform.Rotate(0,180,0);
	}
}
