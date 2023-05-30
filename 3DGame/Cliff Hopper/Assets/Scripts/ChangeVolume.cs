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
    
    
    public void ChangeVolumeButton(){
        if (volume) {
            _img.sprite = _silence;
            _source.PlayOneShot(_uncompressClip);
        }
        else {
            _img.sprite = _sound;
            _source.PlayOneShot(_compressClip);
        }
        volume = !volume;
    }
}
