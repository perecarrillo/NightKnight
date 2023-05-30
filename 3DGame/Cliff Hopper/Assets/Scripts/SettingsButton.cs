using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class SettingsButton : MonoBehaviour
{
    bool stopped = false;

    public void StopTime(){
        if (stopped) Time.timeScale = 1f;
        else Time.timeScale = 0f;
        stopped = !stopped;
    }
}
