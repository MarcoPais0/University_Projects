using UnityEngine;
using System.Collections;
using System.Linq;
using System;

public class CarDetectorGaussScript : CarDetectorScript
{

    public float stdDev = 0.1f;
    public float mean = 0.0f;
	public float response;

    // Get gaussian output value
    public override float GetOutput()
    {
		// calc of Gauss Function (source: Wikipedia)
		float A = (float)(1 / (stdDev * Math.Sqrt(2.0 * Math.PI)));
		response = (float)(A * Math.Exp(-Math.Pow(output - mean, 2) / (2 * Math.Pow(stdDev, 2))));

		if (ApplyThresholds)
		{
			if (output < MinX || output > MaxX)
				response = 0.0f;
		}
		if (ApplyLimits)
		{
			if (response < MinY)
				return MinY;

			if (response > MaxY)
				return MaxY;
		}

		return response;
	}


}
