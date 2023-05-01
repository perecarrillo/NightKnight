using System.Collections;
using System.Collections.Generic;
using UnityEngine;

enum MoveDirection { LEFT, RIGHT };

public class PlayerMovement : MonoBehaviour
{
    Vector3 actPos;
    public float speed;
    bool bReleased;
    float angle;
    MoveDirection currentDirection, moveDirection;
    Vector3 initPos;
    Vector3 movement;

    // Start is called before the first frame update
    void Start()
    {
        // bRotating = false;
        bReleased = true;
        currentDirection = MoveDirection.RIGHT;
        movement = new Vector3 (0, 0, 1);
    }

    // Update is called once per frame
    void Update()
    {
        if (!bReleased && !(Input.GetKey(KeyCode.Space))) {
            bReleased = true;
        }
        if (bReleased && (Input.GetKey(KeyCode.Space))) {
            bReleased = false;
            initPos = transform.position;
            if (currentDirection == MoveDirection.RIGHT) {
                currentDirection = MoveDirection.LEFT;
                transform.Rotate(0.0f, 90.0f, 0.0f);
                moveDirection = MoveDirection.RIGHT;
            }
            else if (currentDirection == MoveDirection.LEFT) {
                currentDirection = MoveDirection.RIGHT;
                transform.Rotate(0.0f, -90.0f, 0.0f);
                moveDirection = MoveDirection.LEFT;
            }
        }
        // move the player
        //transform.Translate(speed * movement * Time.deltaTime);
        if (currentDirection == MoveDirection.RIGHT) transform.position +=  new Vector3(0, 0, speed * Time.deltaTime);
        else if (currentDirection == MoveDirection.LEFT) transform.position +=  new Vector3(speed * Time.deltaTime, 0, 0);

        
    }
}
