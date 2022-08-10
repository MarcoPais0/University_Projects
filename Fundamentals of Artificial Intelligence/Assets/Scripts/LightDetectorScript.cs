using UnityEngine;
using System.Collections;
using System.Linq;
using System;

public class LightDetectorScript : MonoBehaviour {

	public float angle=360;
	public bool ApplyThresholds, ApplyLimits;
	public float MinX, MaxX, MinY, MaxY;
	private bool useAngle = true;

	public float output;
	public int numObjects;

	void Start () {
		output = 0;
		numObjects = 0;

		if (angle > 360) {
			useAngle = false;
		}
	}

	void Update () {
		GameObject[] lights;

		if (useAngle) {
			lights = GetVisibleLights ();
		} else {
			lights = GetAllLights ();
		}

		output = 0;
		numObjects = lights.Length;
	
		foreach (GameObject light in lights) {
			//print (1 / (transform.position - light.transform.position).sqrMagnitude);
			float r = light.GetComponent<Light> ().range;
			output += 1.0f / ((transform.position - light.transform.position).sqrMagnitude / r + 1);
			//Debug.DrawLine (transform.position, light.transform.position, Color.red);
		}
	}

	public virtual float GetOutput() { throw new NotImplementedException(); }

	// Returns all "Light" tagged objects. The sensor angle is not taken into account.
	GameObject[] GetAllLights()
	{
		return GameObject.FindGameObjectsWithTag ("Light");
	}

	// Returns all "Light" tagged objects that are within the view angle of the Sensor. 
	// Only considers the angle over the y axis. Does not consider objects blocking the view.
	GameObject[] GetVisibleLights()
	{
		ArrayList visibleLights = new ArrayList();
		float halfAngle = angle / 2.0f;

		GameObject[] lights = GameObject.FindGameObjectsWithTag ("Light");

		foreach (GameObject light in lights) {
			Vector3 toVector = (light.transform.position - transform.position);
			Vector3 forward = transform.forward;
			toVector.y = 0;
			forward.y = 0;
			float angleToTarget = Vector3.Angle (forward, toVector);

			if (angleToTarget <= halfAngle) {
				visibleLights.Add (light);
			}
		}

		return (GameObject[])visibleLights.ToArray(typeof(GameObject));
	}
}
