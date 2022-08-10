using UnityEngine;
using System.Collections;
using System.Linq;
using System;

public class CarDetectorScript : MonoBehaviour
{

    public float angle = 360;
    public bool ApplyThresholds, ApplyLimits;
    public float MinX, MaxX, MinY, MaxY;
    private bool useAngle = true;

    public float output;
    public int numObjects;

    void Start()
    {
        output = 0;
        numObjects = 0;

        if (angle > 360)
        {
            useAngle = false;
        }
    }

    void Update()
    {
        float min = Mathf.Infinity;

        // YOUR CODE HERE

        GameObject[] cars;
        if (useAngle)
            cars = GetVisibleCars();
        else
            cars = GetAllCars();

        output = 0;

        foreach (GameObject car in cars)
        {
            float dist = Vector3.Distance(transform.position, car.transform.position);
            if (dist < min)
            {
                // searches for the closest car and updates de distance
                GameObject closestCar = car;
                min = dist;
            }
        }

        // checks if the Vehicle2a is moving and calculate the output
        if (min < Mathf.Infinity)
            output = 1.0f / (min + 1.0f);
    }

    public virtual float GetOutput() { throw new NotImplementedException(); }

    // Returns all "Cars" tagged objects. The sensor angle is not taken into account.
    GameObject[] GetAllCars()
    {
        return GameObject.FindGameObjectsWithTag("CarToFollow");
    }

    // YOUR CODE HERE
    GameObject[] GetVisibleCars()
    {
        ArrayList visibleCars = new ArrayList();
        // divides the angle making the only cars visible the ones that are in front of the Vehicle2a 
        float halfAngle = angle / 2;

        // finds all objects with the tag "CarToFollow"
        GameObject[] cars = GameObject.FindGameObjectsWithTag("CarToFollow");

        // iterates through the array of cars
        foreach (GameObject car in cars)
        {
            Vector3 toVector = (car.transform.position - transform.position);
            Vector3 forward = transform.forward;
            toVector.y = 0;
            forward.y = 0;

            float angleToTarget = Vector3.Angle(forward, toVector);

            // checks if the car is in front of the Vehicle2a
            if (angleToTarget <= halfAngle)
                visibleCars.Add(car);
        }

        // returns an array with the visible cars
        return (GameObject[])visibleCars.ToArray(typeof(GameObject));
    }

}
