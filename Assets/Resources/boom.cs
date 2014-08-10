using UnityEngine;
using System.Collections;

public class boom : MonoBehaviour {
	public float radius = 5.0F;
	public float power = 100.0F;
	public void pow() {
		Vector3 explosionPos = transform.position;
		Collider[] colliders = Physics.OverlapSphere(explosionPos, radius);
		foreach (Collider hit in colliders) {
			if (hit.collider.gameObject.GetComponent<Rigidbody>() != null) {
				hit.collider.gameObject.GetComponent<Rigidbody>().AddExplosionForce (power, explosionPos, radius, 1.0f, ForceMode.Impulse);
			}
			
		}
		Destroy (gameObject);
	}
}
