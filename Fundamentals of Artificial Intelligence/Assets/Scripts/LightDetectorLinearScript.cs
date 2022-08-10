using UnityEngine;
using System.Collections;
using System.Linq;
using System;

public class LightDetectorLinearScript : LightDetectorScript {

	public override float GetOutput()
	{
		if (ApplyThresholds)
		{
			if (output < MinX || output > MaxX)
				output = 0.0f;
		}
		if (ApplyLimits)
		{
			if (output < MinY)
				return MinY;

			if (output > MaxY)
				return MaxY;
		}

		return output;
	}
}
