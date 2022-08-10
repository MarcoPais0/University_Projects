using UnityEngine;
using System.Collections;
using System.Linq;
using System;

public class LightDetectorGaussScript : LightDetectorScript {
	public float stdDev = 1.0f; 
	public float mean = 0.0f; 
	public float min_y;
	public bool inverse = false;
	public float response;

	// Get gaussian output value
	public override float GetOutput()
	{
		// calc of Gauss Function (source: Wikipedia)
        float A = (float) (1 / (stdDev * Math.Sqrt(2.0 * Math.PI)));
        response = (float) (A * Math.Exp(-Math.Pow(output - mean, 2) / (2 * Math.Pow(stdDev, 2))));

		if (ApplyThresholds)
		{
			if (output < MinX || output > MaxX)
				response = 0.0f;
		}
		if (ApplyLimits)
		{
			if (response < MinY)
				response = MinY;

			if (response > MaxY)
				response = MaxY;
		}

		if (inverse)
			return 1 - response;
		else
			return response;
	}
}
