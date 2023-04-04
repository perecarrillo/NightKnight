#ifndef _SOUND_CONTROLLER_INCLUDE
#define _SOUND_CONTROLLER_INCLUDE

#include <iostream>
#include <irrKlang.h>
#include <map>

using namespace irrklang;

enum Sounds {CLOCK, LAUGH, MAINTHEME, MENUU, JUMP, WINN, READYY, ARROW, COIN, HEART};

class SoundController
{
private:
	SoundController() {}

public:
	static SoundController& instance()
	{
		static SoundController instance;
		return instance;
	}
	void init();
	void play(Sounds sound, bool loop = false);
	void stop(int sound);
	void stopAllSounds();
	void setAllSoundsPaused(bool b);


private:
	std::map<Sounds, ISoundSource*> sounds;
	ISoundEngine* engine;

};

#endif