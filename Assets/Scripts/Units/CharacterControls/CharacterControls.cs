using UnityEngine;
using System.Collections;

[RequireComponent (typeof (Rigidbody))]
[RequireComponent (typeof (CapsuleCollider))]

public class CharacterControls : MonoBehaviour {
	
	public float speed = 2.0f;
	public float maxVelocityChange = 10.0f;

	public Vector3 targetVelocity = Vector3.zero;

	public bool grounded = false;

	void Awake () {
		rigidbody.freezeRotation = true;
	}
	
	void FixedUpdate () {
		if (grounded) {
			if (targetVelocity != Vector3.zero) {
				GameObject model = transform.Find("ModelRig").gameObject;
				model.transform.rotation = Quaternion.LookRotation(targetVelocity);
				Vector3 ea = model.transform.eulerAngles;
				ea.x = -90.0f;
				model.transform.eulerAngles = ea;
			}

			// Apply a force that attempts to reach our target velocity
			Vector3 velocity = rigidbody.velocity;
			Vector3 velocityChange = (targetVelocity * speed - velocity);
			velocityChange.x = Mathf.Clamp(velocityChange.x, -maxVelocityChange, maxVelocityChange);
			velocityChange.z = Mathf.Clamp(velocityChange.z, -maxVelocityChange, maxVelocityChange);
			velocityChange.y = 0;
			rigidbody.AddForce(velocityChange, ForceMode.VelocityChange);
		}
		grounded = false;
	}

	public void Stop() {
		rigidbody.velocity = Vector3.zero;
		rigidbody.angularVelocity = Vector3.zero;
		targetVelocity = Vector3.zero;
	}
	
	void OnCollisionStay () {
		grounded = true;    
	}
}