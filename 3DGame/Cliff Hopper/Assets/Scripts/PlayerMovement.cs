using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

enum MoveDirection { LEFT, RIGHT };

public class PlayerMovement : MonoBehaviour
{
    Vector3 actPos;
    public float speed;
    bool bReleased;
    MoveDirection currentDirection;
    Vector3 initPos;
    Vector3 movement;
    Rigidbody rb;
    bool isCorner;
    bool hasRotated;

    bool hasDoubleJump;
    int isGrounded = 0;

    public Vector3 jump;
    float jumpForce = 60.0f;

    public GameObject leftArm, rightArm, leftLeg, rightLeg;
    float time;
    float angle, anglePrev;

    // Start is called before the first frame update
    void Start()
    {
        // bRotating = false;
        // bReleased = true;
        hasDoubleJump = false;
        currentDirection = MoveDirection.RIGHT;
        movement = new Vector3 (0, 0, 1);
        rb = GetComponent<Rigidbody>();

        jump = new Vector3(0.0f, 2.0f, 0.0f);
        angle = anglePrev = 0.0f;
    }


    private void OnTriggerEnter(Collider other)
    {
        //Debug.Log("Collision with " + other.gameObject.tag);
        switch (other.gameObject.tag)
        {
            case "Corner":
                isCorner = true;
                return;
            case "Spike":

                SceneManager.LoadScene("SampleScene", LoadSceneMode.Single);
                return;
            case "Stair":
                ++isGrounded;
                return;
            default:
                return;
        }
    }

    private void OnTriggerExit(Collider other) {
        switch (other.gameObject.tag)
        {
            case "Corner":
                isCorner = false;
                hasRotated = false;
                return;
            case "Spike":
                return;
            case "Stair":
                --isGrounded;
                return;
            default:
                return;
        }
    }

    void OnCollisionEnter(Collision other) {
        if (other.gameObject.tag == "Stair") {
            ++isGrounded;
        }
        hasDoubleJump = false;
    }

    void OnCollisionExit(Collision other) {
        float lastY = other.gameObject.transform.position.y;
        Debug.Log(lastY);
        if (other.gameObject.tag == "Stair") {
            --isGrounded;
        }
    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetKeyDown(KeyCode.Space))  {
            initPos = transform.position;
            if (isCorner && !hasRotated) {
                if (currentDirection == MoveDirection.RIGHT) {
                    currentDirection = MoveDirection.LEFT;
                    transform.Rotate(0.0f, 90.0f, 0.0f);
                }
                else if (currentDirection == MoveDirection.LEFT) {
                    currentDirection = MoveDirection.RIGHT;
                    transform.Rotate(0.0f, -90.0f, 0.0f);
                }
                hasRotated = true;
            }
            else if (isGrounded != 0 || hasDoubleJump) {
                //rb.AddForce(0f, jumpSpeed * Time.deltaTime, 0f);
                //rb.AddForce(0, jumpSpeed, 0, ForceMode.Impulse);
                // rb.AddForce(Vector3.up * jumpSpeed,ForceMode.Impulse);
                rb.AddForce(jump * jumpForce, ForceMode.Impulse);
                hasDoubleJump = !hasDoubleJump;
            }
        }

        time += Time.deltaTime;
        anglePrev = angle;
        angle = +90 + 90.0f * Mathf.Sin(time*10);
        leftArm.gameObject.transform.Rotate((angle - anglePrev), 0.0f, 0.0f);
        rightArm.gameObject.transform.Rotate(-(angle - anglePrev), 0.0f, 0.0f);
        leftLeg.gameObject.transform.Rotate(-(angle - anglePrev), 0.0f, 0.0f);
        rightLeg.gameObject.transform.Rotate((angle - anglePrev), 0.0f, 0.0f);


        // move the player
        transform.Translate(speed * movement * Time.deltaTime);
        // if (currentDirection == MoveDirection.RIGHT) transform.position +=  new Vector3(0, 0, speed * Time.deltaTime);
        // else if (currentDirection == MoveDirection.LEFT) transform.position +=  new Vector3(speed * Time.deltaTime, 0, 0);

        
    }
}
