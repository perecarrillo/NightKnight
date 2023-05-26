using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Coin : MonoBehaviour
{

    float speed;
    float time;
    // Start is called before the first frame update
    void Start()
    {
        speed = 60f;
        time = 0;
    }

    // Update is called once per frame
    void Update()
    {
        time += Time.deltaTime;
        transform.Rotate(0.0f, (Time.deltaTime * speed) % 360, 0.0f);
        transform.Translate(0.0f, Mathf.Sin(time/20 * speed)*0.0005f, 0.0f);
    }
}
