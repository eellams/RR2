using UnityEngine;
using System.Collections;



public class UnitMover : MonoBehaviour {
    public bool moveToTarget = false;
	public Vector3 targetPosition = Vector3.zero;

	public float rotationForce = 2.0f;//2.5f;//1.1f; // 2.5
	public float targetReachedRadius = 1.0f;
	public float maxDriveForce = 5.0f;// 14.0
	public float driveForce = 0.0f;
	public float accelerationForce = 6.0f;
	public float stopForce = 10.0f;
	private float distanceToDestiantion = 0.0f;
    private MoveModeType moveMode = MoveModeType.STOP;

	public float PathMulti = 1.0f;
    
    private enum MoveModeType {
        STOP = 1,
        FORWARD = 2	
    };
    
    void Start() {
    
    }
    
    void Update() {
        if (moveToTarget) {
			Vector3 tempTargetPosition = targetPosition;
			tempTargetPosition.y = transform.position.y;
            
			Quaternion rotation = Quaternion.LookRotation(tempTargetPosition - transform.position);
            float str = Mathf.Min (rotationForce * Time.deltaTime, 1); 
            transform.rotation = Quaternion.Lerp(transform.rotation, rotation, str);
            
            // Check direction angle. If greater than 60° then first turn without moving, otherwise full throttle ahead.
			Vector3 targetDir = tempTargetPosition - transform.position;
            Vector3 forward = transform.forward;
            float angle = Vector3.Angle(targetDir, forward);
            if (angle > 40.0) {
                moveMode = MoveModeType.STOP;
            } else {
                moveMode = MoveModeType.FORWARD;
            }

            // check the distance
            distanceToDestiantion = Vector3.Distance(tempTargetPosition, transform.position);
            if (distanceToDestiantion < targetReachedRadius) {
                moveMode = MoveModeType.STOP;
                moveToTarget = false;
                distanceToDestiantion = 0.0f;
            }
        }
    }

	void FixedUpdate () {
		switch (moveMode)  {
			case MoveModeType.STOP:
				driveForce -= stopForce * Time.deltaTime;
				if (driveForce < 0) {
					driveForce = 0;
				}

				break;
			case MoveModeType.FORWARD:
				driveForce += accelerationForce * Time.deltaTime;
				if (driveForce > maxDriveForce) {
					driveForce = maxDriveForce;
				}
				
				var pitchAngle = transform.localEulerAngles.x;
				if (pitchAngle > 180) {
					// vehicle's nose is up, value is below 360°. Normalize to -x degree.
					pitchAngle = -(360 - pitchAngle); // now normalized to (+/-)0-x degree
				}
				break;
			}
		transform.Translate(Vector3.forward * Time.deltaTime * (driveForce ) * PathMulti);
	}
}