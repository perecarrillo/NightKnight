using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BallMovement : MonoBehaviour
{
    public float speed;
    public Vector3 movement;
    // Start is called before the first frame update
    void Start()
    {
        speed = 5f;
        movement = new Vector3(1,0,0);
    }

    // Update is called once per frame
    void Update()
    {
        transform.Translate(speed * movement * Time.deltaTime);
    }
}
