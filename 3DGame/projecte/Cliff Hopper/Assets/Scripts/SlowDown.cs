using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SlowDown : MonoBehaviour
{
    public GameObject player;
    public float slowdownValue = 0.6f;
    void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.tag == "Player") {
            float initialSpeed = player.GetComponent<PlayerMovement>().speed;
            player.GetComponent<PlayerMovement>().speed = initialSpeed*slowdownValue;
        }

    }

    void OnTriggerExit(Collider other)
    {
        if (other.gameObject.tag == "Player") {
            player.GetComponent<PlayerMovement>().setSpeedToInitialSpeed();
        }
    }
}
