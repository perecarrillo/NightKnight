using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class ChangeVolume : MonoBehaviour
{
    bool volume = true;

    [SerializeField] private Image _img;
    [SerializeField] private Sprite _sound, _silence;
    [SerializeField] private AudioClip _compressClip, _uncompressClip;
    [SerializeField] private AudioSource _source;

    void Start() {
        if (PlayerPrefs.GetInt("silence") == 1) {
            volume = false;
            AudioController.instance.TurnOffVolume();
            _img.sprite = _silence;
        }
        else volume = true;
    }
    
    
    public void ChangeVolumeButton(){
        if (volume) {
            _img.sprite = _silence;
            _source.PlayOneShot(_uncompressClip);
            AudioController.instance.TurnOffVolume();
            PlayerPrefs.SetInt("silence", 1);
        }
        else {
            _img.sprite = _sound;
            _source.PlayOneShot(_compressClip);
            AudioController.instance.TurnOnVolume();
            PlayerPrefs.SetInt("silence", 0);
        }
        volume = !volume;
    }
}
