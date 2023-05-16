using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class CubeMovement : MonoBehaviour
{
   [SerializeField] private float _rollSpeed = 5;
    private bool _isMoving;
    MoveDirection currentDirection;
    bool rotate = false;
    
    private void OnTriggerEnter(Collider other)
    {
        //Debug.Log("Collision with " + other.gameObject.tag);
        switch (other.gameObject.tag)
        {
            case "Corner":
                isCorner = true;
                return;
            default:
                return;
        }
        if(other.gameObject.tag == "Corner")
        {
            isCorner = true;
        }
    }

    private void Update() {
        if (_isMoving) return;
 
        // //if (Input.GetKey(KeyCode.A)) Assemble(Vector3.left);
        // else if (Input.GetKey(KeyCode.D)) Assemble(Vector3.right);
        // else if (Input.GetKey(KeyCode.W)) Assemble(Vector3.forward);
        // //else if (Input.GetKey(KeyCode.S)) Assemble(Vector3.back);
        Assemble(Vector3.forward);
 
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
            yield return new WaitForSeconds(0.01f);
        }
        _isMoving = false;
    }
}
