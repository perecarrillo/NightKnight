using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class CubeMovement : MonoBehaviour
{
    [SerializeField] 
    private float rollSpeed = 2;
    private bool isMoving;
    MoveDirection currentDirection = MoveDirection.RIGHT;
    Vector3 direction = Vector3.forward;
    [SerializeField]
    GameObject player;
    float goDown = 0;
    Queue<Collider> alreadyCollided = new Queue<Collider>();

    private void Start() {
        rollSpeed = 100/rollSpeed;
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
                return;
            default:
                return;
        }
    }

    private void FixedUpdate() {
        if (!isMoving) {
            // Update velocity
            float distance = Vector3.Distance(transform.position, player.transform.position);
            if (distance > 10) {
                rollSpeed -= rollSpeed*0.11f;
            }
            else if (distance > 5) {
                rollSpeed -= rollSpeed*0.08f;
            }
            else if (distance < 3) {
                rollSpeed += rollSpeed*0.04f;
                if (rollSpeed == 0) rollSpeed = 0.1f;
            }
            else if (distance < 1) {
                rollSpeed += rollSpeed*0.11f;
            }
            if (goDown > 0) {
                float dist = 0.5f;
                transform.position += Vector3.down*dist;
                goDown -= dist;
            }
            Vector3 anchor = transform.position + (Vector3.down + direction) * 0.5f;
            Vector3 axis = Vector3.Cross(Vector3.up, direction);
            //Debug.Log("Rotate over anchor:" + anchor + " axis: " + axis);
            isMoving = true;
            StartCoroutine(Roll(anchor, axis));
        }
    }
 
    private IEnumerator Roll(Vector3 anchor, Vector3 axis) {
        Debug.Log("Position: " + transform.position);
        float totalDegrees = 90.0f;
        float step = totalDegrees/rollSpeed;
        while (totalDegrees > rollSpeed) {
            transform.RotateAround(anchor, axis, step);
            yield return new WaitForFixedUpdate();
            totalDegrees -= step;
        }
        transform.RotateAround(anchor, axis, totalDegrees);
        yield return new WaitForFixedUpdate();

        // for (int i = 0; i < 90 / rollSpeed; i++) {
        //     // if (goDown > 0) {
        //     //     anchor.y -= 0.1f;
        //     //     goDown -= 0.1f;
        //     // }
        //     transform.RotateAround(anchor, axis, rollSpeed);
        //     yield return new WaitForFixedUpdate();//WaitForSeconds(0.005f);
        // }
        //yield return new WaitForSeconds(0.01f);
        isMoving = false;
        Debug.Log("Position2: " + transform.position);
    }
}
