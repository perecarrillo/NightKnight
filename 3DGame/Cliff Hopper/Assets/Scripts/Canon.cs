using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Canon : MonoBehaviour
{
    public GameObject ballPrefab;

    private void OnTriggerEnter(Collider other)
    {
        switch (other.gameObject.tag)
        {
            case "Player":
                AudioController.instance.Play("Cannon");

                transform.Find("Ball").gameObject.GetComponent<BallMovement>().activeBall();
                return;
        }
    }
}
