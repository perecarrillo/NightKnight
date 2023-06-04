using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class SettingsButton : MonoBehaviour
{
    public void StopTime(){
        Debug.Log("hello");
        if (Time.timeScale == 0f) {
            Debug.Log("maintheme");
            Time.timeScale = 1f;
            AudioController.instance.StopSound("Menu");
            if (PlayerPrefs.GetInt("silence") == 0) AudioController.instance.Play("MainTheme");
            Time.timeScale = 1f;
        }
        else {
            Time.timeScale = 0f;
            AudioController.instance.StopSounds();
            if (PlayerPrefs.GetInt("silence") == 0) AudioController.instance.Play("Menu");
        }
    }

    public void ActiveTime() {
        AudioController.instance.StopSound("Menu");
        if (PlayerPrefs.GetInt("silence") == 0) AudioController.instance.Play("MainTheme");
        Time.timeScale = 1f;
    }
}