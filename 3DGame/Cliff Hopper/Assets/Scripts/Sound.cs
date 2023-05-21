﻿using UnityEngine.Audio;
using UnityEngine;


//Code taken from https://www.youtube.com/watch?v=6OT43pvUyfY

[System.Serializable]
public class Sound {

	public string name;

	public AudioClip clip;

	[Range(0f, 1f)]
	public float volume = .75f;
	//[Range(0f, 1f)]
	[HideInInspector]
	public float volumeVariance = .1f;

	[Range(.1f, 3f)]
	public float pitch = 1f;
	//[Range(0f, 1f)]
	[HideInInspector]
	public float pitchVariance = .1f;

	public bool loop = false;

	//public AudioMixerGroup mixerGroup;

	[HideInInspector]
	public AudioSource source;

}
