using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class CubeMovement : MonoBehaviour
{
    [SerializeField] 
    private float rollSpeed = 2;
    private bool isMoving;
    Vector3 direction = Vector3.forward;
    [SerializeField]
    GameObject player;
    float goDown = 0;
    Queue<Collider> alreadyCollided = new Queue<Collider>();
    float time;

    private void Start() {
        rollSpeed = 100/rollSpeed;
        time = 0f;
    }

    private void OnTriggerEnter(Collider other)
    {
        //Debug.Log("Collision with " + other.gameObject.tag);
        switch (other.gameObject.tag)
        {
            case "Corner":
                if (direction == Vector3.forward) {
                    direction = Vector3.right;
                }
                else {
                    direction = Vector3.forward;
                }
                return;
            case "StairStone":
                if (!alreadyCollided.Contains(other)) {
                    goDown += 0.5f;
                    alreadyCollided.Enqueue(other);
                    if (alreadyCollided.Count >= 10) {
                        alreadyCollided.Dequeue();
                    }
                }
                return;
            case "Player":
                Debug.Log("T'han xafat");
                player.GetComponent<PlayerMovement>().Die();
                return;
            default:
                return;
        }
    }

    private void FixedUpdate() {
        time += Time.deltaTime;
        if (!isMoving && time > 2f) {
            // Update velocity
            float distance = Vector3.Distance(transform.position, player.transform.position);
            //Debug.Log("roolSpeed: " + rollSpeed);
            if (distance > 10 && rollSpeed > 12) {
                rollSpeed -= rollSpeed*0.15f;
            }
            else if (distance > 3 && rollSpeed > 12) {
                rollSpeed -= rollSpeed*0.06f;
            }
            else if (distance < 3) {
                rollSpeed += rollSpeed*0.03f;
                if (rollSpeed == 0) rollSpeed = 0.15f;
            }
            else if (distance < 1.5f) {
                rollSpeed += rollSpeed*0.25f;
            }
            if (goDown > 0) {
                float dist = 0.5f;
                transform.position += Vector3.down*dist;
                goDown -= dist;
            }
            /*if (goDownOnNextRoll) {
                goDown += 0.5f;
                goDownOnNextRoll = false;
            }*/
            Vector3 anchor = transform.position + (Vector3.down + direction) * 0.5f;
            Vector3 axis = Vector3.Cross(Vector3.up, direction);
            //Debug.Log("Rotate over anchor:" + anchor + " axis: " + axis);
            isMoving = true;
            StartCoroutine(Roll(anchor, axis));
        }
    }
 
    private IEnumerator Roll(Vector3 anchor, Vector3 axis) {
        //Debug.Log("Position: " + transform.position);
        float totalDegrees = 90.0f;
        float rest = totalDegrees;
        float step = totalDegrees/rollSpeed; // Avg(Abs(cos)) = 0.637*peak
        while (rest > step) {
        //float step = 
            transform.RotateAround(anchor, axis, step);
            yield return new WaitForFixedUpdate();
            rest -= step;
        }
        transform.RotateAround(anchor, axis, rest);
        yield return new WaitForFixedUpdate();
        isMoving = false;
        //Debug.Log("Position2: " + transform.position);
    }
}