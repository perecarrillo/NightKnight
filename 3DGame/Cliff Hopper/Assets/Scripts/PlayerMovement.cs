using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

enum MoveDirection { LEFT, RIGHT };

public class PlayerMovement : MonoBehaviour
{
    public GameObject Lose;
    public float speed;
    [HideInInspector]
    public Vector3 jump;
    [SerializeField]
    float jumpForce = 60.0f;
    [HideInInspector]
    public float initialSpeed;
    MoveDirection currentDirection;
    Vector3 initPos;
    Vector3 movement;
    [SerializeField]
    Rigidbody rb;
    bool isCorner = false;
    bool hasRotated;

    bool hasFallen = false;
    bool hasDoubleJump = false;
    int isGrounded = 0;
    float lastY = 0;

    public GameObject leftArm, rightArm, leftLeg, rightLeg, body, godBody;
    float time;
    float angle, anglePrev;

    bool dying = false;
    float deathTime;

    int numCoins;

    float recolocate = 0.01f;

    [HideInInspector]
    public bool godMode = false;
    [HideInInspector]
    public bool jumpNext = false;

    // Start is called before the first frame update
    void Start()
    {
        AudioController.instance.Play("MainTheme");
        initialSpeed = speed;
        currentDirection = MoveDirection.RIGHT;
        movement = new Vector3 (0, 0, 1);
        numCoins = 0;

        time = 0f;
        jump = new Vector3(0.0f, 2.0f, 0.0f);
        angle = anglePrev = 0.0f;
    }

    public void Die() {
        if (!dying) {
            FindObjectOfType<AudioController>().Play("Fall");
            deathTime = Time.time;
        }
        dying = true;
    }

    public void setSpeedToInitialSpeed() {
        speed = initialSpeed;
    }

    private void OnTriggerEnter(Collider other)
    {
        //Debug.Log(other.gameObject.tag);
        switch (other.gameObject.tag)
        {
            case "Corner":
                if (godMode && !hasRotated) {
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
                ScoreManager.instance.addPoints(1);
                isCorner = true;
                AudioController.instance.Play("Button");
                other.gameObject.transform.parent.gameObject.transform.Find("buttonNotPressed").gameObject.SetActive(false);
                return;
            case "Spike":
                other.gameObject.transform.Find("pinxos").gameObject.GetComponent<Animator>().SetTrigger("ActivateSpikes");
                rb.velocity = Vector3.zero;
                rb.AddForce(new Vector3(100, 300, 0), ForceMode.Impulse);
                Die();
                //other.transform.gameObject.GetComponent<Animation>().Play("SpikeAnimation");
                //SceneManager.LoadScene("SampleScene", LoadSceneMode.Single);
                return;
            case "Lava":
                AudioController.instance.Play("Lava");
                rb.velocity = Vector3.zero;
                rb.AddForce(new Vector3(0, 300, 0), ForceMode.Impulse);
                Die();
                return;
            case "Ball":
                rb.velocity = Vector3.zero;
                rb.AddForce(new Vector3(0, 0, -20), ForceMode.Impulse);
                Die();
                return;
            case "Stair":
                ++isGrounded;
                return;
            case "Gap":
                Debug.Log("You have fallen");
                hasFallen = true;
                Die();
                return;
            case "Coin":
                //Debug.Log("entra");
                ++numCoins;
                other.gameObject.SetActive(false);
                //FindObjectOfType<AudioController>().Play("Coin");
                AudioController.instance.Play("Coin");
                ScoreManager.instance.addPoints(1);
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
        lastY = other.gameObject.transform.position.y;
        //Debug.Log(lastY);
        --isGrounded;
    }

    // Update is called once per frame
    void Update()
    {
        time += Time.deltaTime;
        if (Input.GetKeyDown(KeyCode.G)) {
            godMode = !godMode;
            body.SetActive(!godMode);
            godBody.SetActive(godMode);
        }
        if (lastY - transform.position.y > 5 && !dying) {
            Debug.Log("T'has caigut huhu");
            Die();
        }
        if (dying) {
            if (Time.time - deathTime >= 1.5f) {
                // SceneManager.LoadScene("SampleScene", LoadSceneMode.Single);
                AudioController.instance.StopSounds();
                if (PlayerPrefs.GetInt("silence") == 0) AudioController.instance.Play("Menu");
                Lose.GetComponent<Animator>().SetTrigger("ShowLose");
            }
        }
        else if (time > 2f) {
            if(!hasFallen && (Input.GetKeyDown(KeyCode.Space) || jumpNext))  {
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
                else if (isGrounded != 0 || hasDoubleJump || jumpNext) {
                    rb.velocity = Vector3.zero;
                    rb.AddForce(jump * jumpForce, ForceMode.Impulse);
                    FindObjectOfType<AudioController>().Play("Jump");
                    hasDoubleJump = !hasDoubleJump;
                    if (isGrounded != 0) hasDoubleJump = true;
                }
                jumpNext = false;
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
