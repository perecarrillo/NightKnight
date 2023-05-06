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
    MoveDirection currentDirection;
    Vector3 initPos;
    Vector3 movement;
    Rigidbody rb;
    bool isCorner;

    public Vector3 jump;
    float jumpForce = 60.0f;

    public bool isGrounded;

    // Start is called before the first frame update
    void Start()
    {
        // bRotating = false;
        // bReleased = true;
        currentDirection = MoveDirection.RIGHT;
        movement = new Vector3 (0, 0, 1);
        rb = GetComponent<Rigidbody>();

        jump = new Vector3(0.0f, 2.0f, 0.0f);
    }


    private void OnCollisionEnter(Collision other)
    {
        if(other.gameObject.tag == "Corner")
        {
            isCorner = true;
        }
        else isCorner = false;
    }

    void OnCollisionStay(){
        isGrounded = true;
    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetKeyDown(KeyCode.Space) && isGrounded)  {
            initPos = transform.position;
            if (isCorner) {
                if (currentDirection == MoveDirection.RIGHT) {
                    currentDirection = MoveDirection.LEFT;
                    transform.Rotate(0.0f, 90.0f, 0.0f);
                }
                else if (currentDirection == MoveDirection.LEFT) {
                    currentDirection = MoveDirection.RIGHT;
                    transform.Rotate(0.0f, -90.0f, 0.0f);
                }
            }
            else {
                //rb.AddForce(0f, jumpSpeed * Time.deltaTime, 0f);
                //rb.AddForce(0, jumpSpeed, 0, ForceMode.Impulse);
                // rb.AddForce(Vector3.up * jumpSpeed,ForceMode.Impulse);
                rb.AddForce(jump * jumpForce, ForceMode.Impulse);
                isGrounded = false;
            }
        }
        // move the player
        transform.Translate(speed * movement * Time.deltaTime);
        // if (currentDirection == MoveDirection.RIGHT) transform.position +=  new Vector3(0, 0, speed * Time.deltaTime);
        // else if (currentDirection == MoveDirection.LEFT) transform.position +=  new Vector3(speed * Time.deltaTime, 0, 0);

        
    }
}
