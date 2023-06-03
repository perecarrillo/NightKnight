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
                // GameObject ball = (GameObject)Instantiate(ballPrefab);
                // ball.transform.Translate(transform.position - new Vector3(4,1,0));
                // Quaternion q = transform.rotation;
                // Vector3 v = q.ToEulerAngles();
                // ball.transform.Rotate(v);
                // ball.transform.parent = transform;

                transform.Find("Ball").gameObject.GetComponent<BallMovement>().activeBall();
                return;
        }
    }
}
