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
                Debug.Log("Shoot ball");
                GameObject ball = (GameObject)Instantiate(ballPrefab);
                ball.transform.Translate(transform.position - new Vector3(-4,0,0));
                ball.transform.parent = transform;
                return;
        }
    }
}
