using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class SettingsButton : MonoBehaviour
{
    public void StopTime(){
        if (Time.timeScale == 0f) Time.timeScale = 1f;
        else Time.timeScale = 0f;
    }

    public void ActiveTime() {
        if (PlayerPrefs.GetInt("silence") == 0) AudioController.instance.Play("MainTheme");
        Time.timeScale = 1f;
    }
}
