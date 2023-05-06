using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class CameraFollow : MonoBehaviour

{
    public Transform target;
    public float smoothSpeed = 0.125f;
    public Vector3 locationOffset;
    public Vector3 rotationOffset;
  
    // void Update()
    // {
    //     transform.position = new Vector3(target.transform.position.x, target.transform.position.y, target.transform.position.z);
    // }

    void FixedUpdate()
    {
        Vector3 desiredPosition = target.position + locationOffset;
        Vector3 smoothedPosition = Vector3.Lerp(transform.position, desiredPosition, smoothSpeed);
        transform.position = smoothedPosition;

        // Quaternion desiredrotation = target.rotation * Quaternion.Euler(rotationOffset);
        // Quaternion smoothedrotation = Quaternion.Lerp(transform.rotation, desiredrotation, smoothSpeed);
        // transform.rotation = smoothedrotation;
    }
}