using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SlowDown : MonoBehaviour
{
    public GameObject player;
    float initialSpeed;
    void OnTriggerEnter(Collider other)
    {
        Debug.Log("asdagsdfg");
        initialSpeed = player.GetComponent<PlayerMovement>().speed;
        player.GetComponent<PlayerMovement>().speed = initialSpeed*0.6f;

    }

    void OnTriggerExit(Collider other)
    {
        player.GetComponent<PlayerMovement>().speed = initialSpeed;
    }
}
