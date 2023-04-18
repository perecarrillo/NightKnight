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

    // Start is called before the first frame update
    void Start()
    {
        // bRotating = false;
        bReleased = true;
        currentDirection = MoveDirection.RIGHT;
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
       
        
    }
}
