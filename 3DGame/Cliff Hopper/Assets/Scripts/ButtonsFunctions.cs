using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;


public class ButtonsFunctions : MonoBehaviour
{
    public void doPlay() {
        SceneManager.LoadScene("SampleScene", LoadSceneMode.Single);
    }

    public void goMainMenu() {
        SceneManager.LoadScene("MainMenu", LoadSceneMode.Single);
    }
}
