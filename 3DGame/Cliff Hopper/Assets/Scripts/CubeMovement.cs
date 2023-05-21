using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class CubeMovement : MonoBehaviour
{
   [SerializeField] private float _rollSpeed = 2;
    private bool _isMoving;
    MoveDirection currentDirection = MoveDirection.RIGHT;
    bool rotate = false;
    Vector3 direction = Vector3.forward;

    
    private void OnTriggerEnter(Collider other)
    {
        //Debug.Log("Collision with " + other.gameObject.tag);
        switch (other.gameObject.tag)
        {
            case "Corner":
                rotate = true;
                return;
            default:
                return;
        }
    }

    private void Update() {
        if (_isMoving) return;
 
        // //if (Input.GetKey(KeyCode.A)) Assemble(Vector3.left);
        // else if (Input.GetKey(KeyCode.D)) Assemble(Vector3.right);
        // else if (Input.GetKey(KeyCode.W)) Assemble(Vector3.forward);
        // //else if (Input.GetKey(KeyCode.S)) Assemble(Vector3.back);
        Assemble(direction);
 
        void Assemble(Vector3 dir) {
            var anchor = transform.position + (Vector3.down + dir) * 0.5f;
            var axis = Vector3.Cross(Vector3.up, dir);
            StartCoroutine(Roll(anchor, axis));
        }
    }
 
    private IEnumerator Roll(Vector3 anchor, Vector3 axis) {
        _isMoving = true;
        for (var i = 0; i < 90 / _rollSpeed; i++) {
            transform.RotateAround(anchor, axis, _rollSpeed);
            yield return new WaitForSeconds(0.001f);
        }
        if(rotate) {
            rotate = false;
            if (currentDirection == MoveDirection.RIGHT) {
                currentDirection = MoveDirection.LEFT;
                direction = Vector3.right;
            }

            else {
                currentDirection = MoveDirection.RIGHT;
                direction = Vector3.forward;
            }

        }
        _isMoving = false;
    }
}
