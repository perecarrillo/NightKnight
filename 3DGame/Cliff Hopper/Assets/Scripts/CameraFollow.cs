using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class CameraFollow : MonoBehaviour

{
    public Transform target;
    public float animationTime = 0.7f;
    float currentTime = 0f;
    public Vector3 desiredPosition;
    public Quaternion desiredRotation;
    public Vector3 initialPosition;
    public Quaternion initialRotation;
    float lastY = 4f;
  
    // void Update()
    // {
    //     transform.position = new Vector3(target.transform.position.x, target.transform.position.y, target.transform.position.z);
    // }

    void FixedUpdate()
    {
        currentTime += Time.deltaTime;
        float alpha = currentTime/animationTime;
        float currentY = target.position.y;
        Debug.Log("currentY" + currentY);
        Debug.Log("lastY" + lastY);
        if (currentY < lastY) lastY = currentY;
        Vector3 targetPosition = new Vector3(target.position.x, lastY, target.position.z);
        //Vector3 smoothedPosition = Vector3.Lerp(transform.position, desiredPosition + targetPosition, alpha);
        Vector3 smoothedPosition = Vector3.Lerp(initialPosition, desiredPosition + targetPosition, alpha);
        transform.position = smoothedPosition;

       // Quaternion desiredrotation = target.rotation;
        //Quaternion smoothedrotation = Quaternion.Lerp(transform.rotation, desiredRotation, alpha);
        Quaternion smoothedrotation = Quaternion.Lerp(initialRotation, desiredRotation, alpha);
        transform.rotation = smoothedrotation;
    }
}