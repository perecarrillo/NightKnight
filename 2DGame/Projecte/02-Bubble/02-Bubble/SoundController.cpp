#include "SoundController.h"

SoundController* SoundController::singleton;

void SoundController::init()
{
	engine = createIrrKlangDevice();
	sounds[LAUGH] = engine->addSoundSourceFromFile("sounds/laugh.wav");
	sounds[MAINTHEME] = engine->addSoundSourceFromFile("sounds/MainTheme.mp3");
	sounds[MENUU] = engine->addSoundSourceFromFile("sounds/Menu.mp3");
	sounds[JUMP] = engine->addSoundSourceFromFile("sounds/Jump.mp3");
	sounds[WINN] = engine->addSoundSourceFromFile("sounds/Win.mp3");
	sounds[READYY] = engine->addSoundSourceFromFile("sounds/Ready.mp3");
	sounds[ARROW] = engine->addSoundSourceFromFile("sounds/Arrow.mp3");
	sounds[COIN] = engine->addSoundSourceFromFile("sounds/coin.mp3");
	sounds[CLOCK] = engine->addSoundSourceFromFile("sounds/fast_clock.mp3");
	sounds[HEART] = engine->addSoundSourceFromFile("sounds/heart.mp3");
	sounds[WINFINAL] = engine->addSoundSourceFromFile("sounds/win2.mp3");
	sounds[OPENING_CHEST] = engine->addSoundSourceFromFile("sounds/opening_chest.mp3");
	sounds[ESCUT] = engine->addSoundSourceFromFile("sounds/escut.mp3");
	sounds[KEY] = engine->addSoundSourceFromFile("sounds/key.mp3");
	sounds[GAME_OVER] = engine->addSoundSourceFromFile("sounds/gameOver.mp3");
	sounds[COLLISION] = engine->addSoundSourceFromFile("sounds/collision.mp3");
	sounds[POP] = engine->addSoundSourceFromFile("sounds/pop3.mp3");
}

void SoundController::play(Sounds id, bool loop) //Loop is optional
{
	//std::cout<<sounds[sound]<<std::endl;
	ISound *sound;
	if (id ==  MENUU || id == MAINTHEME) sound = engine->play2D(sounds[id], true, false, true);
	else sound = engine->play2D(sounds[id], loop, false, true);
	playing[id].push_back(sound);
}

void SoundController::stop(Sounds id) 
{
	if (playing.find(id) == playing.end()) return;
	for (ISound* sound : playing[id])
	{
		sound->stop();
	}
}

void SoundController::stopAll()
{
	engine->stopAllSounds();
}

void SoundController::pauseAll()
{
	engine->setAllSoundsPaused(true);
}

void SoundController::unPauseAll()
{
	engine->setAllSoundsPaused(false);
}

bool SoundController::isPlaying(Sounds sound)
{
	return playing.find(sound) != playing.end();
}

void SoundController::OnSoundStopped(ISound * sound, E_STOP_EVENT_CAUSE reason, void * userData)
{
	for (auto list : playing)
	{
		for (ISound *s : list.second)
		{
			if (s == sound) list.second.remove(s);
		}
		if (list.second.empty())
			playing.erase(list.first);
	}
}
