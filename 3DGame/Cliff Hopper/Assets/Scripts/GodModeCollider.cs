using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GodModeCollider : MonoBehaviour
{
    [SerializeField]
    GameObject player;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }


    void OnTriggerEnter(Collider other) {
        tag = other.gameObject.tag;
        if (transform.parent.GetComponent<PlayerMovement>().godMode && (tag == "Spike" || tag == "Gap")) {
            //Debug.Log("Collided with " + tag);
            transform.parent.GetComponent<PlayerMovement>().jumpNext = true;
        }
    }
}
