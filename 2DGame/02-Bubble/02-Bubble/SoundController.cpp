#include "SoundController.h"

void SoundController::init()
{
	engine = createIrrKlangDevice();
	sounds[LAUGH] = engine->addSoundSourceFromFile("sounds/laugh.wav");
	sounds[CLOCK] = engine->addSoundSourceFromFile("sounds/clock.wav");
	sounds[MAINTHEME] = engine->addSoundSourceFromFile("sounds/MainTheme.mp3");
	sounds[MENUU] = engine->addSoundSourceFromFile("sounds/Menu.mp3");
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
