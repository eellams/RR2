using UnityEngine;
using System.Collections;

// Thanks to hamcav and aldonaletto from
// http://answers.unity3d.com/questions/242648/force-on-character-controller-knockback.html


public class ImpactReceiver : MonoBehaviour {
	float mass = 3.0f;
	Vector3 impact = Vector3.zero;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		if (impact.magnitude > 0.2f) {
			CharacterController cc = GetComponent<CharacterController> ();
			cc.Move (impact*Time.deltaTime);
			impact = Vector3.Lerp (impact, Vector3.zero, Time.deltaTime);
		}
	}

	public void AddImpact(Vector3 dir, float force) {
		dir.Normalize ();
		if (dir.y < 0)
			dir.y = -dir.y;
		impact += dir.normalized * force / mass;
	}
}
