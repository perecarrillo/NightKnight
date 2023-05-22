using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

enum MoveDirection { LEFT, RIGHT };

public class PlayerMovement : MonoBehaviour
{
    public float speed;
    [HideInInspector]
    public float initialSpeed;
    MoveDirection currentDirection;
    Vector3 initPos;
    Vector3 movement;
    Rigidbody rb;
    bool isCorner;
    bool hasRotated;

    bool hasFallen = false;
    bool hasDoubleJump = false;
    int isGrounded = 0;

    public Vector3 jump;
    float jumpForce = 60.0f;

    public GameObject leftArm, rightArm, leftLeg, rightLeg;
    float time;
    float angle, anglePrev;

    bool dying = false;
    float deathTime;

    float recolocate = 0.005f;

    // Start is called before the first frame update
    void Start()
    {
        initialSpeed = speed;
        currentDirection = MoveDirection.RIGHT;
        movement = new Vector3 (0, 0, 1);
        rb = GetComponent<Rigidbody>();

        jump = new Vector3(0.0f, 2.0f, 0.0f);
        angle = anglePrev = 0.0f;
    }

    public void setSpeedToInitialSpeed() {
        speed = initialSpeed;
    }

    private void OnTriggerEnter(Collider other)
    {
        //Debug.Log("Collision with " + other.gameObject.tag);
        switch (other.gameObject.tag)
        {
            case "Corner":
                isCorner = true;
                other.gameObject.transform.parent.gameObject.transform.Find("buttonNotPressed").gameObject.SetActive(false);
                return;
            case "Spike":
                dying = true;
                deathTime = Time.time;
                FindObjectOfType<AudioController>().Play("laugh");
                //other.transform.gameObject.GetComponent<Animation>().Play("SpikeAnimation");
                //SceneManager.LoadScene("SampleScene", LoadSceneMode.Single);
                return;
            case "Stair":
                ++isGrounded;
                return;
            case "Gap":
                Debug.Log("You have fallen");
                hasFallen = true;
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
        if (hasFallen) {
            Debug.Log("Deleted collision");
            Destroy(other.gameObject.GetComponent<Collider>());
        }
        ++isGrounded;
        hasDoubleJump = false;
    }

    void OnCollisionExit(Collision other) {
        float lastY = other.gameObject.transform.position.y;
        //Debug.Log(lastY);
        --isGrounded;
    }

    // Update is called once per frame
    void Update()
    {
        if (dying) {
            if (Time.time - deathTime >= 5) {
                SceneManager.LoadScene("SampleScene", LoadSceneMode.Single);
            }
        }
        else {
            if(!hasFallen && Input.GetKeyDown(KeyCode.Space))  {
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


            // move the player and center the player 
            transform.Translate(speed * movement * Time.deltaTime);
        
            if (currentDirection == MoveDirection.RIGHT) {
                float pos = transform.position.x;
                int goal = (int)Mathf.Round(pos);
                float deviation = pos - goal;
                deviation = Mathf.Abs(deviation);
                if (deviation < recolocate){
                    if (goal > pos) transform.Translate(deviation, 0, 0);
                    else if (goal > pos) transform.Translate(-deviation, 0, 0);
                }
                else if (pos < goal) {
                    transform.Translate(recolocate, 0, 0);
                }
                else if (pos > goal) {
                    transform.Translate(-recolocate, 0, 0);
                }
            }
            else {
                float pos = transform.position.z;
                int goal = (int)Mathf.Round(pos);
                float deviation = pos - goal;
                deviation = Mathf.Abs(deviation);
                if (deviation < recolocate){
                    if (goal > pos) transform.Translate(-deviation, 0, 0);
                    else if (goal > pos) transform.Translate(deviation, 0, 0);
                }
                else if (pos < goal) {
                    transform.Translate(-recolocate, 0, 0);
                }
                else if (pos > goal) {
                    transform.Translate(recolocate, 0, 0);
                }
            }
            
        

            
        }
    }
}
