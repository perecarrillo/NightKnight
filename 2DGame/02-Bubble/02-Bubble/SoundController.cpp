#include "SoundController.h"

void SoundController::init()
{
	engine = createIrrKlangDevice();
	sounds[LAUGH] = engine->addSoundSourceFromFile("sounds/laugh.wav");
	sounds[CLOCK] = engine->addSoundSourceFromFile("sounds/clock.wav");
	sounds[MAINTHEME] = engine->addSoundSourceFromFile("sounds/MainTheme.mp3");
	sounds[MENUU] = engine->addSoundSourceFromFile("sounds/Menu.mp3");
	sounds[JUMP] = engine->addSoundSourceFromFile("sounds/Jump.mp3");
	sounds[WINN] = engine->addSoundSourceFromFile("sounds/Win.mp3");
	sounds[READYY] = engine->addSoundSourceFromFile("sounds/Ready.mp3");
	sounds[ARROW] = engine->addSoundSourceFromFile("sounds/Arrow.mp3");
	sounds[COIN] = engine->addSoundSourceFromFile("sounds/coin.mp3");
}

void SoundController::play(Sounds sound, bool loop) //Loop is optional
{
	engine->play2D(sounds[sound]);
}

void SoundController::stop(int sound) 
{
}

void SoundController::stopAllSounds()
{
	engine->stopAllSounds();
}

void SoundController::setAllSoundsPaused(bool b)
{
	engine->setAllSoundsPaused(b);
}
