using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SlowDown : MonoBehaviour
{
    public GameObject player;
    float initialSpeed;
    public float slowdownValue = 0.6f;
    void OnTriggerEnter(Collider other)
    {
        initialSpeed = player.GetComponent<PlayerMovement>().speed;
        player.GetComponent<PlayerMovement>().speed = initialSpeed*slowdownValue;

    }

    void OnTriggerExit(Collider other)
    {
        player.GetComponent<PlayerMovement>().speed = initialSpeed;
    }
}
