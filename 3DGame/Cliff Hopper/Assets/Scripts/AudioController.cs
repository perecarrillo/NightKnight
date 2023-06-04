using UnityEngine.Audio;
using System;
using UnityEngine;


//Code taken from https://www.youtube.com/watch?v=6OT43pvUyfY
public class AudioController : MonoBehaviour
{

	public static AudioController instance;

	//public AudioMixerGroup mixerGroup;

	//No es un diccionari pq unity no els pot dibuixar a l'inspector
	public Sound[] sounds;
	public float volume = 1f;

	void Awake()
	{
		if (instance != null)
		{
			Destroy(gameObject);
		}
		else
		{
			instance = this;
			DontDestroyOnLoad(gameObject);
		}

		foreach (Sound s in sounds)
		{
			s.source = gameObject.AddComponent<AudioSource>();
			s.source.clip = s.clip;
			s.source.loop = s.loop;

			//s.source.outputAudioMixerGroup = mixerGroup;
		}
	}

	public void Play(string sound)
	{
		Sound s = Array.Find(sounds, item => item.name == sound);
		if (s == null)
		{
			Debug.LogWarning("Sound: " + name + " not found!");
			return;
		}

		s.source.volume = volume * s.volume * (1f + UnityEngine.Random.Range(-s.volumeVariance / 2f, s.volumeVariance / 2f));
		s.source.pitch = volume * s.pitch * (1f + UnityEngine.Random.Range(-s.pitchVariance / 2f, s.pitchVariance / 2f));

		//s.source.PlayOneShot(s.source.clip);
		s.source.Play();
	}

	public void TurnOnVolume() {
		volume = 1;
	}

	public void TurnOffVolume() {
		volume = 0;
	}

}
