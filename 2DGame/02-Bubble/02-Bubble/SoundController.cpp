#include "SoundController.h"

void SoundController::init()
{
	engine = createIrrKlangDevice();
	sounds[LAUGH] = engine->addSoundSourceFromFile("sounds/laugh.wav");
	sounds[CLOCK] = engine->addSoundSourceFromFile("sounds/clock.wav");
}

void SoundController::play(Sounds sound, bool loop) //Loop is optional
{
	engine->play2D(sounds[sound]);
}

void SoundController::stop(int sound) 
{

}
