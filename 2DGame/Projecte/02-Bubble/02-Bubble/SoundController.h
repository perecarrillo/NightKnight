#pragma single
#ifndef _SOUND_CONTROLLER_INCLUDE
#define _SOUND_CONTROLLER_INCLUDE

#include <iostream>
#include <irrKlang.h>
#include <map>
#include <list>

using namespace irrklang;

enum Sounds {CLOCK, LAUGH, MAINTHEME, MENUU, JUMP, WINN, READYY, ARROW, COIN, HEART, WINFINAL, OPENING_CHEST, ESCUT, KEY, GAME_OVER, COLLISION, POP};

class SoundController : public ISoundStopEventReceiver
{
private:
	SoundController() {}

public:
	static SoundController* instance()
	{
		if (singleton == nullptr)
			singleton = new SoundController();
		return singleton;
	}
	void init();
	void play(Sounds id, bool loop = false);
	void stop(Sounds id);
	void stopAll();
	void pauseAll();
	void unPauseAll();
	bool isPlaying(Sounds sound);
	void OnSoundStopped(ISound* sound, E_STOP_EVENT_CAUSE reason, void* userData);



private:
	static SoundController* singleton;
	std::map<Sounds, ISoundSource*> sounds;
	ISoundEngine* engine;
	std::map<Sounds, std::list<ISound*>> playing;

};

#endif